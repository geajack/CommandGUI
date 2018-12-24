#include <QSizePolicy>
#include <QColor>
#include <QTextOption>
#include <QTextCursor>
#include <QScrollBar>
#include <QDebug>
#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(QWidget* parent) : QTextEdit(parent)
{
    setMinimumHeight(80);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    
    setStyleSheet("background-color: black; color: #EEE; font-family: monospace");
    setReadOnly(true);
    setWordWrapMode(QTextOption::WrapAnywhere);
    
    cursor = textCursor();  
    cursor.insertHtml("<b>&gt;</b> ");
    input = "";
    running = false;
    
    qProc = new QProcess();
    connect(qProc, SIGNAL(readyReadStandardError()), this, SLOT(printError()));
    connect(qProc, SIGNAL(readyReadStandardOutput()), this, SLOT(printOutput()));
    connect(qProc, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));
    connect(qProc, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
}

void TerminalWidget::setInput(QString s)
{
    if (! input.isEmpty())
    {
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, input.length());
        cursor.removeSelectedText();
    }
    
    cursor.insertHtml("<b>" + s + "</b>");
    input = s;
    scrollToBottom();
}

bool TerminalWidget::isRunning()
{
    return running;
}

void TerminalWidget::killProcess()
{
    qProc->kill();
}

void TerminalWidget::sendInput()
{
    qDebug() << "sendInput called";
    running = true;
    cursor.insertHtml("<br />");
    scrollToBottom();
    input = "";
    qProc -> start(input);
}

QString TerminalWidget::getInput()
{
    return input;
}

void TerminalWidget::onProcessError(QProcess::ProcessError error)
{
    switch (error)
    {
        case QProcess::FailedToStart:
            qDebug() << "Process failed to start";
        break;
        
        case QProcess::Crashed:
            qDebug() << "Process crashed";
        break;
        
        case QProcess::Timedout:
            qDebug() << "Process timed out";
        break;

        case QProcess::WriteError:
            qDebug() << "Process suffered a write error";
        break;

        case QProcess::ReadError:
            qDebug() << "Process suffered a read error";
        break;

        case QProcess::UnknownError:
            qDebug() << "Process suffered an unknown error";
        break;
    }

    onFinished(1);
}

void TerminalWidget::onFinished(int exitStatus)
{
    qDebug() << "Process finished";
    cursor.insertHtml("<br /><b>&gt;</b> ");
    running = false;
    emit readyForInput();
}

void TerminalWidget::printError()
{
    qDebug() << "Got standard error";
    QString s = qProc -> readAllStandardError();
    cursor.insertText(s);
    scrollToBottom();
}

void TerminalWidget::printOutput()
{
    qDebug() << "Got standard output";
    QString s = qProc -> readAllStandardOutput();
    cursor.insertText(s);
    scrollToBottom();
}

void TerminalWidget::scrollToBottom()
{
    verticalScrollBar() -> setValue(verticalScrollBar() -> maximum());
}
