#include <gtkmm.h>
#include "ApplicationGTK.h"
#include "ui/gtk/MainWindow.h"

int ApplicationGTK::execute()
{
    auto app = Gtk::Application::create("com.github.geajack.commandgui");

    MainWindow mainWindow;

    return app->run(mainWindow);
}