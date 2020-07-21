#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow()
{
    set_default_size(350, 350);
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
    std::cout << commandID << "\n";

    CommandDescriptor *commandDescriptor = commandManager.getCommandDescriptor(commandID);
    commandPage.reset();
    commandPage.loadCommandDescriptor(commandDescriptor);

    tabs.set_current_page(1);
}

void MainWindow::onClickBack()
{
    tabs.set_current_page(0);
}