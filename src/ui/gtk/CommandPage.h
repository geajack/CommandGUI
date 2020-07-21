#include <gtkmm.h>
#include "../../core/CommandDescriptor.h"

class CommandPage : public Gtk::Box
{
    public:
        CommandPage();
        void loadCommandDescriptor(CommandDescriptor *descriptor);
        void onClickBack();
        sigc::signal<void> signal_clicked_back;
};