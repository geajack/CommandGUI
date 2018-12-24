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
    cursor.insertHtml("<br />");
    qProc -> start(input);
    input = "";
    running = true;
    scrollToBottom();
}

QString TerminalWidget::getInput()
{
    return input;
}

void TerminalWidget::onFinished(int exitStatus)
{
    cursor.insertHtml("<br /><b>&gt;</b> ");
    running = false;
    emit readyForInput();
}

void TerminalWidget::printError()
{
    QString s = qProc -> readAllStandardError();
    cursor.insertText(s);
    scrollToBottom();
}

void TerminalWidget::printOutput()
{
    QString s = qProc -> readAllStandardOutput();
    cursor.insertText(s);
    scrollToBottom();
}

void TerminalWidget::scrollToBottom()
{
    verticalScrollBar() -> setValue(verticalScrollBar() -> maximum());
}
