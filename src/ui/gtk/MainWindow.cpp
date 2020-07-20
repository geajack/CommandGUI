#include "MainWindow.h"
#include "CommandPage.h"
#include "../../core/CommandManager.h"
#include <iostream>

MainWindow::MainWindow()
{
    set_default_size(350, 350);
    set_title("Command GUI");

    set_border_width(5);

    auto *tabs = new Gtk::Notebook;
    // tabs->set_show_tabs(false);
    add(*tabs);

    Gtk::Box *mainPage = new Gtk::Box;
    mainPage->set_orientation(Gtk::ORIENTATION_VERTICAL);
    tabs->add(*mainPage);

    CommandPage *commandPage = new CommandPage;
    tabs->add(*commandPage);

    CommandManager commandManager;
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

    show_all_children();
}

void MainWindow::onCommandClicked(int commandID)
{
    std::cout << commandID << "\n";
}