#include <QApplication>
#include "ui/MainWindow.h"

int main(int, char**);

int WinMain(int argc, char *argv[])
{
    return main(argc, argv);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}