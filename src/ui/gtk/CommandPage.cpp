#include "CommandPage.h"
#include "../../core/CommandTemplateParser.h"
#include <iostream>
#include <thread>

gboolean updateUI(gpointer data);

CommandPage::CommandPage(Gtk::Window *parent)
{
    Gtk::ScrolledWindow *scrollingArea = new Gtk::ScrolledWindow;
    Gtk::Paned *panedView = new Gtk::Paned;
    Gtk::Button *button = new Gtk::Button;

    this->parent = parent;

    panedView->set_orientation(Gtk::ORIENTATION_VERTICAL);

    scrollingArea->set_vexpand(true);

    contentArea.set_orientation(Gtk::ORIENTATION_VERTICAL);
    contentArea.set_row_spacing(5);
    contentArea.set_column_spacing(15);
    contentArea.set_margin_bottom(10);

    button->set_label("Back");
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &CommandPage::onClickBack)
    );

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

    set_orientation(Gtk::ORIENTATION_VERTICAL);

    scrollingArea->add(contentArea);
    panedView->pack1(*scrollingArea, true, false);
    panedView->pack2(terminal, false, false);
    add(*panedView);
    add(*button);
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
            sigc::mem_fun(*this, &CommandPage::onChangeValue)
        );

        textEntries[variable->name] = inputField;
    }

    onChangeValue();

    contentArea.show_all_children();

    std::thread *thread = new std::thread(&CommandPage::runCommand, this);
}

void CommandPage::reset()
{
    for (auto child : contentArea.get_children())
    {
        contentArea.remove(*child);
    }

    textEntries.clear();
}

void CommandPage::onClickBack()
{
    signal_clicked_back.emit();
}

void CommandPage::onChangeValue()
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
    terminal.get_buffer()->set_text(shellCommand);
}

void CommandPage::runCommand()
{
    gdk_threads_add_idle(updateUI, this);
}

gboolean updateUI(gpointer data)
{
    CommandPage *commandPage = (CommandPage*) data;

    std::cout << "Hello, world!" << "\n";
    return 0;
}