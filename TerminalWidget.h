#pragma once

#include <QProcess>
#include <QTextEdit>
#include <QTextCursor>
#include <QString>

class TerminalWidget : public QTextEdit
{
        Q_OBJECT
        QProcess* qProc;
        bool running;
        QString input;
        QTextCursor cursor;
        
        void scrollToBottom();
        
    public:        
        TerminalWidget(QWidget* parent);
        void setInput(QString s);
        void sendInput();
        bool isRunning();
        void killProcess();
        QString getInput();
        
    public slots:        
        void printOutput();
        void printError();
        void onFinished(int exitStatus);
                        
    signals:                        
        void readyForInput();
};
