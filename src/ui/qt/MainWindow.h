#pragma once

#include <QMainWindow>
#include <QStackedLayout>
#include <QWidget>
#include <QCloseEvent>
#include "../../core/CommandManager.h"
#include "CommandPage.h"
#include "MenuPage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QStackedLayout* layout;
    CommandManager* commandManager;
    CommandPage* commandPage;
    MenuPage* menuPage;
    
    void showErrorMessage(QString message);
    void showMenu();
    void closeEvent(QCloseEvent* e);
    
    public:
        explicit MainWindow(QWidget* parent = 0);
        ~MainWindow();
    
    public slots:
        void selectCommand(int id);
        void backRequested();
};