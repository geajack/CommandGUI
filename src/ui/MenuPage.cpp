#include <QString>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QScrollArea>
#include <QList>
#include "../CommandDescriptor.h"
#include "MenuPage.h"

MenuPage::MenuPage(QWidget* parent) : QScrollArea(parent)
{  
    QWidget* contentBox = new QWidget();
    layout = new QVBoxLayout();
    contentBox -> setLayout(layout);
    
    setWidget(contentBox);
    setWidgetResizable(true);
    
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(choseItem(int)));
}

void MenuPage::loadCommandHeaders(QList<CommandHeader>* headers)
{
    QPushButton* button;
    for (int i = 0; i < headers->length(); i++)
    {
        button = new QPushButton(QString::fromStdString(*headers->at(i).name));
        layout -> addWidget(button);
        signalMapper -> setMapping(button, i);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
    }
    layout -> addStretch();
}
