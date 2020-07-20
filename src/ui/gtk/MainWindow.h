#include <gtkmm.h>

class MainWindow : public Gtk::Window
{
    public:
        MainWindow();
        void onCommandClicked(int commandID);
};