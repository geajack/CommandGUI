#include "Application.h"

int main(int, char**);

int WinMain(int argc, char *argv[])
{
    return main(argc, argv);
}

int main(int argc, char *argv[])
{
    Application application;

    return application.execute();
}