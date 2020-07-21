#include <gtkmm.h>
#include "../../core/CommandDescriptor.h"

class CommandPage : public Gtk::Box
{
    public:
        CommandPage(Gtk::Window *parent);
        void reset();
        void loadCommandDescriptor(CommandDescriptor *descriptor);
        void onClickBack();
        void onChangeValue();
        sigc::signal<void> signal_clicked_back;
    private:
        Gtk::Window *parent;
        CommandDescriptor commandDescriptor;
        Gtk::Grid contentArea;
        Gtk::TextView terminal;
        std::map<std::string*, Gtk::Entry*> textEntries;
};