#include <gtkmm.h>
#include "../../core/Process.h"
#include "../../core/CommandDescriptor.h"

class CommandPage : public Gtk::Box
{
    public:
        CommandPage(Gtk::Window *parent);
        void loadCommandDescriptor(CommandDescriptor *descriptor);
        void reset();
        void render();

        void monitorChildProcess();
        void onClickBack();
        void onFormChanged();
        void onClickExecute();

        sigc::signal<void> signal_clicked_back;
    private:
        Gtk::Window *parent;
        Gtk::Grid contentArea;
        Gtk::TextView terminal;
        std::map<std::string*, VariableEntryWidgetGTK*> textEntries;
        CommandDescriptor commandDescriptor;

        std::string command;
        std::string output;
        Process *process;

        void generateCommand();
};