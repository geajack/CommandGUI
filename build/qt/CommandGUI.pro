######################################################################
# Automatically generated by qmake (3.1) Fri Sep 20 21:59:38 2019
######################################################################

TEMPLATE = app
TARGET = CommandGUI
INCLUDEPATH += .
INCLUDEPATH += ../../lib/include

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += COMMANDGUI_UI_PLATFORM_QT

# Input
HEADERS += ../../src/core/CommandDescriptor.h \
           ../../src/core/CommandManager.h \
           ../../src/ui/qt/CommandPage.h \
           ../../src/core/CommandTemplateParser.h \
           ../../src/core/Exceptions.h \
           ../../src/ui/qt/FileChooserButton.h \
           ../../src/ui/qt/FormWidget.h \
           ../../src/ui/qt/MainWindow.h \
           ../../src/ui/qt/MenuPage.h \
           ../../src/ui/qt/TerminalWidget.h \
           ../../src/Application.h \
           ../../src/ApplicationQt.h \
           ../../lib/include/cJSON.h
SOURCES += ../../src/core/CommandDescriptor.cpp \
           ../../src/core/CommandManager.cpp \
           ../../src/ui/qt/CommandPage.cpp \
           ../../src/core/CommandTemplateParser.cpp \
           ../../src/ui/qt/FileChooserButton.cpp \
           ../../src/ui/qt/FormWidget.cpp \
           ../../src/main.cpp \
           ../../src/ui/qt/MainWindow.cpp \
           ../../src/ui/qt/MenuPage.cpp \
           ../../src/ui/qt/TerminalWidget.cpp \
           ../../src/ApplicationQt.cpp \
           ../../lib/cJSON.c

QT += widgets
DESTDIR = ../../bin
OBJECTS_DIR=artifacts
MOC_DIR=artifacts
CONFIG+=debug
CONFIG+=c++17