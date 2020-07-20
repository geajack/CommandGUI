#include "MainWindow.h"
#include "../../core/CommandManager.h"
#include <iostream>

MainWindow::MainWindow()
{
    set_default_size(350, 350);
    set_title("Command GUI");

    set_border_width(5);

    Gtk::Box *box = new Gtk::Box;
    box->set_orientation(Gtk::ORIENTATION_VERTICAL);
    add(*box);

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
        box->add(*button);
    }

    show_all_children();
}