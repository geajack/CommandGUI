#include <gtk/gtk.h>
#include "ApplicationGTK.h"

static void on_application_start(GtkApplication *app, gpointer user_data)
{
    GtkWidget *main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Tasks");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 400);

    gtk_widget_show_all(main_window);
}

int ApplicationGTK::execute()
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.github.geajack.tasks", G_APPLICATION_FLAGS_NONE); 
    g_signal_connect(app, "activate", G_CALLBACK(on_application_start), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    return status;
}