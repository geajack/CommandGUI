#include <gtkmm.h>
#include "ApplicationGTK.h"

int ApplicationGTK::execute()
{
    auto app = Gtk::Application::create("com.github.geajack.commandgui");

    Gtk::Window window;
    window.set_default_size(350, 350);
    window.set_title("Command GUI");

    return app->run(window);
}