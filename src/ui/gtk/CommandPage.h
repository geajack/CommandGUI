#include <gtkmm.h>
#include "../../core/Process.h"
#include "../../core/CommandDescriptor.h"

class CommandPage : public Gtk::Box
{
    public:
        Process *process;

        CommandPage(Gtk::Window *parent);
        void reset();
        void loadCommandDescriptor(CommandDescriptor *descriptor);
        void onClickBack();
        void onChangeValue();
        void runCommand();
        void renderOutput(std::string output);
        void monitorChildProcess();
        sigc::signal<void> signal_clicked_back;
    private:
        Gtk::Window *parent;
        CommandDescriptor commandDescriptor;
        Gtk::Grid contentArea;
        Gtk::TextView terminal;
        std::map<std::string*, VariableEntryWidgetGTK*> textEntries;
};