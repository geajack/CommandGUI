#include <gtkmm.h>
#include "../../core/CommandManager.h"
#include "CommandPage.h"

class MainWindow : public Gtk::Window
{
    public:
        MainWindow();
        void onCommandClicked(int commandID);
    private:
        CommandManager commandManager;
        Gtk::Notebook tabs;
        CommandPage commandPage;
};