#include "MainWindow.h"

MainWindow::MainWindow(): commandPage(this)
{
    set_default_size(450, 500);
    set_title("Command GUI");

    set_border_width(10);

    tabs.set_show_tabs(false);
    tabs.set_show_border(false);

    Gtk::Box *mainPage = new Gtk::Box;
    mainPage->set_orientation(Gtk::ORIENTATION_VERTICAL);   

    commandPage.signal_clicked_back.connect(
        sigc::mem_fun(*this, &MainWindow::onClickBack)
    );

    commandManager.initialize("/home/jack/Code/CommandGUI/Codebase/bin/commands");
    
    auto headers = commandManager.getHeaders();
    for (int i = 0; i < headers->size(); i++)
    {
        CommandHeader header = headers->at(i);
        Gtk::Button *button = new Gtk::Button(*header.name);
        button->set_vexpand(false);
        if (i > 0)
        {
            button->set_margin_top(5);
        }
        button->signal_clicked().connect(
            sigc::bind<int>(
                sigc::mem_fun(*this, &MainWindow::onCommandClicked),
                header.id
            )
        );
        mainPage->add(*button);
    }

    tabs.add(*mainPage);
    tabs.add(commandPage);
    add(tabs);

    show_all_children();
}

void MainWindow::onCommandClicked(int commandID)
{
    CommandDescriptor *commandDescriptor = commandManager.getCommandDescriptor(commandID);
    if (commandDescriptor != NULL)
    {
        commandPage.reset();
        commandPage.loadCommandDescriptor(commandDescriptor);
        tabs.set_current_page(1);
    }
    else
    {
        Gtk::MessageDialog *popup = new Gtk::MessageDialog(
            *this,
            commandManager.getErrorMessage(),
            false,
            Gtk::MESSAGE_ERROR,
            Gtk::BUTTONS_OK,
            true
        );
        popup->set_title("Uh oh!");
        popup->run();
        delete popup;
    }
}

void MainWindow::onClickBack()
{
    tabs.set_current_page(0);
}