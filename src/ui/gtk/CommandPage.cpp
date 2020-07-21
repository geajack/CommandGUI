#include "CommandPage.h"
#include <iostream>

CommandPage::CommandPage()
{   
    Gtk::ScrolledWindow *scrollingArea = new Gtk::ScrolledWindow;
    Gtk::Paned *panedView = new Gtk::Paned;
    Gtk::Button *button = new Gtk::Button;

    panedView->set_orientation(Gtk::ORIENTATION_VERTICAL);

    scrollingArea->set_vexpand(true);

    contentArea.set_orientation(Gtk::ORIENTATION_VERTICAL);
    contentArea.set_row_spacing(5);
    contentArea.set_column_spacing(15);

    button->set_label("Back");
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &CommandPage::onClickBack)
    );

    terminal.set_size_request(-1, 80);
    terminal.set_margin_bottom(10);

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

    for (int i = 0; i < nVariables; i++)
    {
        VariableDescriptor *variable = descriptor->variableList->at(i);
        Gtk::Label *label = new Gtk::Label(*(variable->label));
        Gtk::Entry *inputField = new Gtk::Entry;

        label->set_hexpand(false);
        inputField->set_hexpand(true);
        contentArea.attach(*label, 0, i, 1, 1);
        contentArea.attach(*inputField, 1, i, 1, 1);
    }
    contentArea.show_all_children();
}

void CommandPage::reset()
{
    for (auto child : contentArea.get_children())
    {
        delete child;
    }
}

void CommandPage::onClickBack()
{
    signal_clicked_back.emit();
}