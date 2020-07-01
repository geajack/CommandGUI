#pragma once

#include <string.h>
#include <QString>
#include <QList>
#include <QMap>
#include <QJsonDocument>

class VariableDescriptor
{
    public:
        struct MultipleChoiceItem
        {
            QString label;
            QString value;
        };
        
        QString* name;
        QString* label;
        QString defaultValue;
        int type;
        QList<VariableDescriptor::MultipleChoiceItem>* choices;
        
        static const int TYPE_STRING = 0;
        static const int TYPE_BOOLEAN = 1;
        static const int TYPE_FILE = 2;
        static const int TYPE_FOLDER = 3;
        static const int TYPE_MULTIPLE_CHOICE = 4;
        
        static VariableDescriptor* FromJSON(QJsonObject qJsonObj, QString* errorMessage);
};

class CommandDescriptor
{
    public:
        QString name;
        QString templateString;
        QList<VariableDescriptor*>* variableList;
        QMap<QString, VariableDescriptor*>* variableMap;
        
        QList<VariableDescriptor*>* getVariableList();
        QMap<QString, VariableDescriptor*>* getVariableMap();
        
        static CommandDescriptor* FromJSON(QJsonObject qJsonObj, QString* errorMessage);
};

struct CommandHeader
{
    int id;
    std::string* name;
};
