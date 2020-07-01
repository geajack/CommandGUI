#pragma once

#include <string.h>
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
        
        std::string* name;
        std::string* label;
        std::string defaultValue;
        int type;
        QList<VariableDescriptor::MultipleChoiceItem>* choices;
        
        static const int TYPE_STRING = 0;
        static const int TYPE_BOOLEAN = 1;
        static const int TYPE_FILE = 2;
        static const int TYPE_FOLDER = 3;
        static const int TYPE_MULTIPLE_CHOICE = 4;
        
        static VariableDescriptor* FromJSON(QJsonObject qJsonObj, std::string* errorMessage);
};

class CommandDescriptor
{
    public:
        std::string name;
        std::string templateString;
        QList<VariableDescriptor*>* variableList;
        QMap<std::string, VariableDescriptor*>* variableMap;
        
        QList<VariableDescriptor*>* getVariableList();
        QMap<std::string, VariableDescriptor*>* getVariableMap();
        
        static CommandDescriptor* FromJSON(QJsonObject qJsonObj, std::string* errorMessage);
};

struct CommandHeader
{
    int id;
    std::string* name;
};
