#include <QMainWindow>
#include <QStackedLayout>
#include <QMessageBox>
#include <QList>
#include <QCloseEvent>
#include <QDebug>
#include "MenuPage.h"
#include "CommandPage.h"
#include "../CommandManager.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Command GUI");
    resize(350, 350);
    
    commandManager = new CommandManager();
    commandManager -> initialize();
    
    if (commandManager -> getError() == X_OKAY)
    {
        QList<CommandHeader>* headers = commandManager -> getHeaders();
        
        QWidget* centralWidget = new QWidget();
        layout = new QStackedLayout();
        menuPage = new MenuPage(this);
        commandPage = 0;
        
        connect(menuPage, SIGNAL(choseItem(int)), this, SLOT(selectCommand(int)));
        
        layout -> addWidget(menuPage);		
        centralWidget -> setLayout(layout);    
        setCentralWidget(centralWidget);
        
        menuPage->loadCommandHeaders(headers);
        showMenu();
    }
    else
    {
        showErrorMessage(commandManager->getErrorMessage());
    }
}

void MainWindow::selectCommand(int id)
{
    qDebug() << "selectCommand begins";
    try
    {
        CommandDescriptor* commandDescriptor = commandManager -> getCommandDescriptor(id);
        
        if (commandManager->getError() != X_OKAY)
        {
            throw commandManager->getErrorMessage();
        }
        
        commandPage = new CommandPage(this);
        commandPage->loadCommandDescriptor(commandDescriptor);
        
        if (commandPage->getError() != X_OKAY)
        {
            throw commandPage->getErrorMessage();
        }
        
        connect(commandPage, SIGNAL(back()), this, SLOT(backRequested()));
        layout->addWidget(commandPage);
        layout->setCurrentIndex(1);
    }
    catch (QString msg)
    {
        showErrorMessage(msg);
        showMenu();
    }
    qDebug() << "selectCommand ends";
}

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox* messageBox = new QMessageBox(this);
    messageBox->setWindowTitle("Uh-oh");
    messageBox->setText(message);
    messageBox->show();
}

void MainWindow::showMenu()
{
    layout->setCurrentIndex(0);
    
    if (commandPage)
    {
        delete commandPage;
        commandPage = 0;
    }
}

void MainWindow::backRequested()
{
    if (! commandPage->isRunning())
    {
        showMenu();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Hold on!",
            "If you leave now, the currently running process will be killed! Do you really want to leave?",
            QMessageBox::Yes|QMessageBox::No
        );
        if (reply == QMessageBox::Yes)
        {
            commandPage -> killProcess();
            showMenu();        
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    if (commandPage && commandPage->isRunning())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Hold on!",
            "If you leave now, the currently running process will be killed! Do you really want to leave?",
            QMessageBox::Yes|QMessageBox::No
        );
        if (reply == QMessageBox::Yes)
        {
            commandPage -> killProcess();
            e->accept();
        }
        else
        {
            e->ignore();
        }
    }
    else
    {
        e->accept();
    }
}

MainWindow::~MainWindow()
{
}
