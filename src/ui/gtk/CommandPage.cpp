#include "CommandPage.h"

CommandPage::CommandPage()
{   
    Gtk::ScrolledWindow *scrollingArea = new Gtk::ScrolledWindow;
    Gtk::Button *button = new Gtk::Button;

    scrollingArea->set_vexpand(true);

    contentArea.set_orientation(Gtk::ORIENTATION_VERTICAL);
    contentArea.set_vexpand(false);
    contentArea.set_hexpand(false);

    button->set_label("Back");
    button->set_vexpand(false);
    button->signal_clicked().connect(
        sigc::mem_fun(*this, &CommandPage::onClickBack)
    );

    set_orientation(Gtk::ORIENTATION_VERTICAL);

    scrollingArea->add(contentArea);
    add(*scrollingArea);
    add(*button);
}

void CommandPage::loadCommandDescriptor(CommandDescriptor *descriptor)
{
    auto name = descriptor->name;
    auto nVariables = descriptor->variableList->size();

    for (int i = 0; i < nVariables; i++)
    {
        Gtk::Label *label = new Gtk::Label(name);
        label->set_vexpand(false);
        label->set_hexpand(true);
        contentArea.pack_start(*label, Gtk::PACK_SHRINK);
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