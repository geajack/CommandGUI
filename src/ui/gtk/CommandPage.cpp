#include "CommandPage.h"

CommandPage::CommandPage()
{   
    Gtk::ScrolledWindow *scrollingArea = new Gtk::ScrolledWindow;
    Gtk::Box *contentArea = new Gtk::Box;
    Gtk::Button *button = new Gtk::Button;

    scrollingArea->set_vexpand(true);

    contentArea->set_orientation(Gtk::ORIENTATION_VERTICAL);
    contentArea->set_vexpand(false);
    contentArea->set_hexpand(false);

    button->set_label("Back");
    button->set_vexpand(false);
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &CommandPage::onClickBack)
    );

    for (int i = 0; i < 10; i++)
    {
        Gtk::Label *label = new Gtk::Label("Hello");
        label->set_vexpand(false);
        label->set_hexpand(true);
        contentArea->pack_start(*label, Gtk::PACK_SHRINK);
    }

    set_orientation(Gtk::ORIENTATION_VERTICAL);

    scrollingArea->add(*contentArea);
    add(*scrollingArea);
    add(*button);
}

void CommandPage::loadCommandDescriptor(CommandDescriptor *descriptor)
{
    auto name = descriptor->name;
    auto nVariables = descriptor->variableList->size();

    // label.set_text(name);
}

void CommandPage::onClickBack()
{
    signal_clicked_back.emit();
}