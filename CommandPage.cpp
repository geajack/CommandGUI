#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSplitter>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QScrollArea>
#include <QClipboard>
#include <QApplication>
#include "CommandPage.h"
#include "FormWidget.h"
#include "CommandDescriptor.h"
#include "CommandTemplateParser.h"
#include "TerminalWidget.h"
#include "FileChooserButton.h"

CommandPage::CommandPage(QWidget *parent) : QWidget(parent)
{
    qDebug() << "CommandPage constructor begins";
    exceptionCode = X_OKAY;
    
    /////////////////////
    ///   Presentation
    /////////////////////
    
    // Top level containers
    QSplitter* splitter = new QSplitter();
    splitter -> setOrientation(Qt::Vertical);
    contentBox = new QScrollArea(this);
    terminalWidget = new TerminalWidget(this);
    QWidget* buttonBox  = new QWidget(this);
    
    // Content box
    formBox = 0;
    textEntry = 0;
    contentBox -> setWidgetResizable(true);
    splitter -> addWidget(contentBox);
    splitter -> addWidget(terminalWidget);
    
    // Button box
    executeButton = new QPushButton("Execute");
    copyButton    = new QPushButton("Copy");
    backButton    = new QPushButton("Back");
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(executeButton);
    buttonLayout->addWidget(copyButton);
    buttonLayout->addWidget(backButton);
    buttonBox->setLayout(buttonLayout);
    
    // Main layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout -> addWidget(splitter);
    layout -> addWidget(buttonBox);
    setLayout(layout);
    
    /////////////////////
    ///   Behavior
    /////////////////////
    
    connect(executeButton, SIGNAL(clicked()), this, SLOT(executeClicked()));
    connect(copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(backButton, SIGNAL(clicked()), this, SIGNAL(back()));

    qDebug() << "CommandPage constructor ends";
}

void CommandPage::loadCommandDescriptor(CommandDescriptor* command)
{    
    qDebug() << "loadCommandDescriptor begins";
    commandDescriptor = command;
    
    formBox = new FormWidget();    
    
    QList<VariableDescriptor*>* variables = command -> getVariableList();
    QString defaultValue;
    VariableDescriptor* vd;
    
    for (int i = 0; i < variables -> length(); i++)
    {
        vd = variables -> at(i);
        defaultValue = vd->defaultValue;
        
        switch (vd -> type)
        {
            case VariableDescriptor::TYPE_STRING:
                formBox -> addTextItem(*(vd -> name), *(vd -> label), defaultValue);
            break;
            
            case VariableDescriptor::TYPE_BOOLEAN:
                if (defaultValue == "true")
                {
                    formBox -> addBooleanItem(*(vd -> name), *(vd -> label), true);
                }
                else if (defaultValue == "false")
                {
                    formBox -> addBooleanItem(*(vd -> name), *(vd -> label), false);
                }
                else
                {
                    qDebug() << "Uh oh";
                }
            break;
                
            case VariableDescriptor::TYPE_FILE:
                formBox -> addFileItem(*(vd -> name), *(vd -> label), defaultValue, FileChooserButton::FILE);
            break;
                
            case VariableDescriptor::TYPE_FOLDER:
                formBox->addFileItem(*(vd -> name), *(vd -> label), defaultValue, FileChooserButton::FOLDER);
            break;
                
            case VariableDescriptor::TYPE_MULTIPLE_CHOICE:
                {
                    QList<DropDownListEntry>* entryList = new QList<DropDownListEntry>;
                    DropDownListEntry entry;
                    for (int i = 0; i < vd -> choices -> length(); i++)
                    {
                        entry.label = (vd -> choices -> at(i) ).label;
                        entry.value = (vd -> choices -> at(i) ).value;
                        entryList -> append(entry);
                    }                     
                    formBox -> addDropDownItem(*(vd -> name), *(vd -> label), defaultValue, entryList);
                }
            break;

            default:
                qDebug() << "Command page encountered bad VariableDescriptor";
            break;
        }
    }
    
    formBox -> setMinimumWidth(300);
    contentBox -> setWidget(formBox);
    
    updateTerminalInput();
    
    connect(formBox, SIGNAL(formChanged()), this, SLOT(onFormChanged()));
    connect(terminalWidget, SIGNAL(readyForInput()), this, SLOT(updateTerminalInput()));

    qDebug() << "loadCommandDescriptor ends";
}

void CommandPage::copy()
{
    QClipboard* cb = QApplication::clipboard();
    cb->setText(terminalWidget->getInput());
}

void CommandPage::updateTerminalInput()
{
    qDebug() << "updateTerminalInput begins";
    QMap<QString, QString>* dataMap = formBox -> getFormData();
    
    CommandTemplateParser parser = CommandTemplateParser(commandDescriptor, dataMap);
    
    parser.parse();
    
    if (parser.getError() == X_OKAY)
    {
        terminalWidget -> setInput(parser.getResult());
    }
    else
    {
        exceptionCode = parser.getError();
        errorMessage = parser.getErrorMessage();
    }
    qDebug() << "updateTerminalInput ends";
}

void CommandPage::killProcess()
{
    terminalWidget->killProcess();
}

bool CommandPage::isRunning()
{
    return terminalWidget->isRunning();
}

void CommandPage::onFormChanged()
{
    updateTerminalInput();
}

void CommandPage::executeClicked()
{
    terminalWidget -> sendInput();
}

ExceptionCode CommandPage::getError()
{
    return exceptionCode;
}

QString CommandPage::getErrorMessage()
{
    return errorMessage;
}

CommandPage::~CommandPage()
{
    
}
