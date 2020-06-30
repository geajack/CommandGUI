#include <QApplication>

#include "Application.h"
#include "ui/MainWindow.h"

int Application::execute()
{
    int argc = 0;
    QApplication a(argc, NULL);
    MainWindow w;
    w.show();

    return a.exec();
}