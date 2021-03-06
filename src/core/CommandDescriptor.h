#pragma once

#include <string.h>
#include <vector>
#include <map>
#include "cJSON.h"
#include "Exceptions.h"
#include "MultipleChoiceItem.h"
#include "../ui/VariableEntryWidget.h"

class VariableDescriptor
{
    public:      
        std::string* name;
        std::string* label;
        std::string defaultValue;
        int type;
        std::vector<MultipleChoiceItem>* choices;
        
        static const int TYPE_STRING = 0;
        static const int TYPE_BOOLEAN = 1;
        static const int TYPE_FILE = 2;
        static const int TYPE_FOLDER = 3;
        static const int TYPE_MULTIPLE_CHOICE = 4;
        
        static VariableDescriptor* FromJSON(cJSON *json, std::string* errorMessage);

        VariableEntryWidget* buildEntryWidget();
};

class CommandDescriptor
{
    public:
        std::string name;
        std::string templateString;
        std::vector<VariableDescriptor*>* variableList;
        
        std::vector<VariableDescriptor*>* getVariableList();
        VariableDescriptor* getVariable(std::string name);
        
        static CommandDescriptor* FromJSON(std::string *filePath, ExceptionCode *exceptionCode, std::string *errorMessage);
    private:
        std::map<std::string, VariableDescriptor*>* variableMap;
};

struct CommandHeader
{
    int id;
    std::string* name;
};
