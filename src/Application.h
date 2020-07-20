#ifdef COMMANDGUI_UI_PLATFORM_QT

    #include "ApplicationQt.h"
    typedef ApplicationQt Application;

#endif

#ifdef COMMANDGUI_UI_PLATFORM_GTK

    #include "ApplicationGTK.h"
    typedef ApplicationGTK Application;

#endif