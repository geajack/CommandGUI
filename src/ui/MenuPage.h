#pragma once

#include <QWidget>
#include <QList>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QScrollArea>
#include "../core/CommandDescriptor.h"
#include "../core/Exceptions.h"

class MenuPage : public QScrollArea
{
    Q_OBJECT
    QVBoxLayout* layout;
    QSignalMapper* signalMapper;
    
    public:
        explicit MenuPage(QWidget *parent = 0);
        void loadCommandHeaders(std::vector<CommandHeader>* headers);
        
    signals:
        void choseItem(int id);
};