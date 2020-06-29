#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QScrollArea>
#include "FormWidget.h"
#include "../CommandDescriptor.h"
#include "TerminalWidget.h"
#include "../Exceptions.h"

class CommandPage : public QWidget
{   
        Q_OBJECT
        QScrollArea* contentBox;
        QVBoxLayout* contentLayout;
        FormWidget* formBox;
        QPushButton* executeButton;
        QPushButton* copyButton;
        QPushButton* backButton;
        CommandDescriptor* commandDescriptor;
        QLineEdit* textEntry;
        TerminalWidget* terminalWidget;
        ExceptionCode exceptionCode;
        QString errorMessage;
        
    public:
        explicit CommandPage(QWidget *parent = 0);
        ~CommandPage();
        void loadCommandDescriptor(CommandDescriptor* command);
        ExceptionCode getError();
        QString getErrorMessage();
        bool isRunning();
        void killProcess();
        
    signals:
        void back();
        void execute(QString command);
        
    public slots:
        void executeClicked();
        void updateTerminalInput();
        void onFormChanged();
        void copy();
};