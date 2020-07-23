#include "CommandPage.h"
#include "../../core/CommandTemplateParser.h"
#include <iostream>
#include <thread>

gboolean onOutputWritten(gpointer data);
gboolean onFinish(gpointer data);

CommandPage::CommandPage(Gtk::Window *parent)
{
    command = "";
    output = "";
    commandRunning = false;

    Gtk::ScrolledWindow *scrollingArea = new Gtk::ScrolledWindow;
    Gtk::Paned *panedView = new Gtk::Paned;
    Gtk::ButtonBox *buttonBox = new Gtk::ButtonBox;
    Gtk::Button *backButton = new Gtk::Button;

    this->parent = parent;

    panedView->set_orientation(Gtk::ORIENTATION_VERTICAL);

    scrollingArea->set_vexpand(true);

    contentArea.set_orientation(Gtk::ORIENTATION_VERTICAL);
    contentArea.set_row_spacing(5);
    contentArea.set_column_spacing(15);
    contentArea.set_margin_bottom(10);    

    terminal.set_size_request(-1, 80);
    terminal.set_margin_bottom(10);
    terminal.set_editable(false);
    terminal.get_buffer()->set_text("Hello, world!");
    terminal.set_wrap_mode(Gtk::WRAP_WORD);

    terminal.get_style_context()->add_class("terminal");
    auto styleProvider = Gtk::CssProvider::create();
    auto cssFile = Gio::File::create_for_path("/home/jack/Code/CommandGUI/Codebase/resources/gtk/style.css");
    styleProvider->load_from_file(cssFile);
    terminal.get_style_context()->add_provider(styleProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    buttonBox->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    buttonBox->set_layout(Gtk::BUTTONBOX_EXPAND);
    buttonBox->add(stopStartButton);
    buttonBox->add(*backButton);

    backButton->signal_clicked().connect(
        sigc::mem_fun(*this, &CommandPage::onClickBack)
    );
    backButton->set_hexpand(true);
    backButton->set_label("Back");
    
    stopStartButton.signal_clicked().connect(
        sigc::mem_fun(*this, &CommandPage::onClickExecute)
    );
    stopStartButton.set_hexpand(true);
    stopStartButton.set_label("Execute");

    set_orientation(Gtk::ORIENTATION_VERTICAL);

    scrollingArea->add(contentArea);
    panedView->pack1(*scrollingArea, true, false);
    panedView->pack2(terminal, false, false);
    add(*panedView);
    add(*buttonBox);
}

void CommandPage::loadCommandDescriptor(CommandDescriptor *descriptor)
{
    auto name = descriptor->name;
    auto nVariables = descriptor->variableList->size();

    commandDescriptor = *descriptor;

    for (int i = 0; i < nVariables; i++)
    {
        VariableDescriptor *variable = descriptor->variableList->at(i);
        Gtk::Label *label = new Gtk::Label(*(variable->label));
        VariableEntryWidgetGTK *inputField = variable->buildEntryWidget();

        label->set_hexpand(false);
        inputField->getWidget()->set_hexpand(true);
        contentArea.attach(*label, 0, i, 1, 1);
        contentArea.attach(*inputField->getWidget(), 1, i, 1, 1);

        inputField->onChangeSignal.connect(
            sigc::mem_fun(*this, &CommandPage::onFormChanged)
        );

        textEntries[variable->name] = inputField;
    }

    generateCommand();
    render();

    contentArea.show_all_children();
}

void CommandPage::reset()
{
    for (auto child : contentArea.get_children())
    {
        contentArea.remove(*child);
    }

    textEntries.clear();

    output = "";
}

void CommandPage::generateCommand()
{
    CommandTemplateParser parser(&commandDescriptor);
    for (auto pair : textEntries)
    {
        std::string *variableName = pair.first;
        VariableEntryWidgetGTK *entryWidget = pair.second;
        std::string value = entryWidget->getStringValue();

        parser.addVariable(variableName, &value);
    }

    parser.parse();

    std::string shellCommand = parser.getResult();
    command = shellCommand;
}

void CommandPage::render()
{
    std::string text = "$ " + command + "\n" + output;
    terminal.get_buffer()->set_text(text);
}

void CommandPage::onClickExecute()
{
    if (!commandRunning)
    {
        std::string command = terminal.get_buffer()->get_text();
        stopStartButton.set_label("Stop");
        commandRunning = true;
        process = new Process(command);
        process->run();
        std::thread *thread = new std::thread(&CommandPage::monitorChildProcess, this);
    }
    else
    {
        process->stop();
    }
}

void CommandPage::onClickBack()
{
    bool reallyGoBack = true;
    if (commandRunning)
    {
        std::string message = "There is a command running! If you exit now, the command will be stopped. Really exit to main menu?";
        Gtk::MessageDialog *popup = new Gtk::MessageDialog(
            *parent,
            message,
            false,
            Gtk::MESSAGE_WARNING,
            Gtk::BUTTONS_YES_NO,
            true
        );
        int response = popup->run();
        delete popup;

        if (response == Gtk::RESPONSE_YES)
        {
            process->stop();
            commandRunning = false;
            stopStartButton.set_label("Execute");
        }
        else
        {
            reallyGoBack = false;
        }
    }

    if (reallyGoBack)
    {
        signal_clicked_back.emit();
    }
}

void CommandPage::onFormChanged()
{
    if (!commandRunning)
    {
        generateCommand();
        render();
    }
}

void CommandPage::onProcessCompleted()
{
    commandRunning = false;
    stopStartButton.set_label("Execute");
}

void CommandPage::monitorChildProcess()
{
    while (process->isRunning())
    {
        output = process->getOutput();
        gdk_threads_add_idle(onOutputWritten, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delete process;

    gdk_threads_add_idle(onFinish, this);
}

gboolean onOutputWritten(gpointer data)
{
    CommandPage *commandPage = (CommandPage*) data;
    commandPage->render();
    return 0;
}

gboolean onFinish(gpointer data)
{
    CommandPage *commandPage = (CommandPage*) data;
    commandPage->onProcessCompleted();
    return 0;
}