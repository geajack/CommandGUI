#include <QApplication>

#include "ApplicationQt.h"
#include "ui/qt/MainWindow.h"

int ApplicationQt::execute()
{
    int argc = 0;
    QApplication a(argc, NULL);
    MainWindow w;
    w.show();

    return a.exec();
}