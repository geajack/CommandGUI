#include <fstream>
#include "CommandDescriptor.h"
#include "CommandTemplateParser.h"

CommandDescriptor* CommandDescriptor::FromJSON(std::string *filePath, ExceptionCode *exceptionCode, std::string *errorMessage)
{
    CommandDescriptor* cd = new CommandDescriptor();

    cJSON *json = NULL;
    std::ifstream in = std::ifstream(*filePath, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        json = cJSON_Parse(contents.c_str());
        in.close();
    }
    else
    {
        *exceptionCode = X_FILE_UNREADABLE;
        *errorMessage  = "There was a problem with the file " + *filePath + ". ";
        *errorMessage  += "Is it corrupted or missing?";
        return 0;
    }
    
    if (json == NULL)
    {
        *exceptionCode = X_BAD_JSON_SYNTAX;
        *errorMessage = "The given file contains invalid JSON. " + std::string(cJSON_GetErrorPtr());
        return 0;
    }

    if (json->type != cJSON_Object)
    {
        *errorMessage = "The root entity of the file was not a JSON object.";
        return 0;
    }

    if (!cJSON_HasObjectItem(json, "name"))
    {
        *errorMessage = "There was no name.";
        return 0;
    }

    cJSON *nameJSON = cJSON_GetObjectItem(json, "name");
    if (!cJSON_IsString(nameJSON))
    {
        *errorMessage = "The name value was not a string.";
        return 0;
    }

    if (!cJSON_HasObjectItem(json, "templateString"))
    {
        *errorMessage = "There was no template string.";
        return 0;
    }

    cJSON *templateStringJSON = cJSON_GetObjectItem(json, "templateString");
    if (!cJSON_IsString(templateStringJSON))
    {
        *errorMessage = "The template string value was not a string.";
        return 0;
    }

    if (!cJSON_HasObjectItem(json, "variables"))
    {
        *errorMessage = "The variables array was not present.";
        return 0;
    }

    cJSON *variablesJSON = cJSON_GetObjectItem(json, "variables");
    if (!cJSON_IsArray(variablesJSON))
    {
        *errorMessage = "The 'variables' key does not contain an array.";
        return 0;
    }

    cd -> name = std::string(cJSON_GetStringValue(nameJSON));
    cd -> templateString = std::string(cJSON_GetStringValue(templateStringJSON));
    cd -> variableMap = new std::map<std::string, VariableDescriptor*>;
    cd -> variableList = new std::vector<VariableDescriptor*>;

    VariableDescriptor* vd;
    std::string variableErrorString;
    int nVariables = cJSON_GetArraySize(variablesJSON);
    for (int i = 0; i < nVariables; i++)
    {
        cJSON *item = cJSON_GetArrayItem(variablesJSON, i);
        vd = VariableDescriptor::FromJSON(item, &variableErrorString);

        if (vd == 0)
        {
            *errorMessage = "There was a problem parsing variable number " + std::to_string(i + 1) + " of your variable list. ";
            *errorMessage += variableErrorString;
            return 0;
        }

        (*(cd -> variableMap))[*(vd -> name)] = vd;
        cd -> variableList -> push_back(vd);
    }

    CommandTemplateParser parser = CommandTemplateParser(cd);
    ExceptionCode status = parser.validate();
    
    if (status == X_OKAY)
    {
        *exceptionCode = X_OKAY;
        *errorMessage = "";
        return cd;
    }
    else
    {
        *exceptionCode = parser.getError();
        *errorMessage = parser.getErrorMessage();
        return NULL;
    }
}

std::vector<VariableDescriptor*>* CommandDescriptor::getVariableList()
{
    return variableList;
}

VariableDescriptor* CommandDescriptor::getVariable(std::string name)
{
    return (*variableMap)[name];
}

VariableEntryWidget* VariableDescriptor::buildEntryWidget()
{
    bool initialValue;
    switch (type)
    {
        case TYPE_BOOLEAN:
            initialValue = false;
            if (defaultValue == "true")
            {
                initialValue = true;
            }
            return new BooleanVariableEntryWidget(initialValue);
        break;

        case TYPE_STRING:
            return new StringVariableEntryWidget(defaultValue);
        break;

        case TYPE_MULTIPLE_CHOICE:
            return new MultipleChoiceVariableEntryWidget(choices, defaultValue);
        break;

        case TYPE_FILE:
            return new FileVariableEntryWidget(defaultValue);
        break;

        case TYPE_FOLDER:
            return new FolderVariableEntryWidget(defaultValue);
        break;

        default:
            return NULL;
        break;
    }
}

VariableDescriptor* VariableDescriptor::FromJSON(cJSON *json, std::string *errorString)
{
    VariableDescriptor* vd = new VariableDescriptor();

    if (json->type != cJSON_Object)
    {
        *errorString = "The variable is not a JSON object.";
        return 0;
    }

    if (!cJSON_HasObjectItem(json, "type"))
    {
        *errorString = "The variable had no \"type\" attribute.";
        return 0;
    }

    cJSON *typeJSON = cJSON_GetObjectItem(json, "type");
    if (!cJSON_IsString(typeJSON))
    {
        *errorString = "The variable's \"type\" attribute was not a string.";
        return 0;
    }

    if (!cJSON_HasObjectItem(json, "name"))
    {
        *errorString = "The variable had no \"name\" attribute.";
        return 0;
    }

    cJSON *nameJSON = cJSON_GetObjectItem(json, "name");
    if (!cJSON_IsString(nameJSON))
    {
        *errorString = "The variable's \"name\" attribute was not a string.";
        return 0;
    }

    if (!cJSON_HasObjectItem(json, "label"))
    {
        *errorString = "The variable had no \"label\" attribute.";
        return 0;
    }

    cJSON *labelJSON = cJSON_GetObjectItem(json, "label");
    if (!cJSON_IsString(labelJSON))
    {
        *errorString = "The variable's \"label\" attribute was not a string.";
        return 0;
    }

    std::string type = cJSON_GetStringValue(typeJSON);

    if (type == "string")
    {
        vd -> type = VariableDescriptor::TYPE_STRING;
        vd -> defaultValue = "";
    }
    else if (type == "boolean")
    {
        vd -> type = VariableDescriptor::TYPE_BOOLEAN;
        vd -> defaultValue = "false";
    }
    else if (type == "file")
    {
        vd -> type = VariableDescriptor::TYPE_FILE;
        vd -> defaultValue = "";
    }
    else if (type == "folder")
    {
        vd->type = VariableDescriptor::TYPE_FOLDER;
        vd -> defaultValue = "";
    }
    else if (type == "multiple")
    {
        if (!cJSON_HasObjectItem(json, "choices"))
        {
            *errorString = "The variable is a multiple choice type variable, but has no \"choices\" array.";
            return 0;
        }

        cJSON *choicesJSON = cJSON_GetObjectItem(json, "choices");
        if (!cJSON_IsArray(choicesJSON))
        {
            *errorString = "The variable is a multiple choice type variable, and the \"choices\" attribute is not an array.";
            return 0;
        }

        vd -> type = VariableDescriptor::TYPE_MULTIPLE_CHOICE;
        vd -> defaultValue = "";
        vd -> choices = new std::vector<MultipleChoiceItem>;
        cJSON *choiceJSON;
        int nChoices = cJSON_GetArraySize(choicesJSON);
        MultipleChoiceItem item;
        for (int i = 0; i < nChoices; i++)
        {
            choiceJSON = cJSON_GetArrayItem(choicesJSON, i);

            if (choiceJSON->type != cJSON_Object)
            {
                *errorString = "A member of the \"choices\" array is not an object.";
                return 0;
            }

            if (!cJSON_HasObjectItem(choiceJSON, "value"))
            {
                *errorString = "A member of the \"choices\" array has no \"value\" attribute.";
                return 0;
            }

            cJSON *valueJSON = cJSON_GetObjectItem(choiceJSON, "value");
            if (!cJSON_IsString(valueJSON))
            {
                *errorString = "A member of the \"choices\" array has a non-string \"value\" attribute.";
                return 0;
            }

            if (!cJSON_HasObjectItem(choiceJSON, "label"))
            {
                *errorString = "A member of the \"choices\" array has no \"label\" attribute.";
                return 0;
            }

            cJSON *labelJSON = cJSON_GetObjectItem(choiceJSON, "label");
            if (!cJSON_IsString(labelJSON))
            {
                *errorString = "A member of the \"choices\" array has a non-string \"label\" attribute.";
                return 0;
            }

            item.value = cJSON_GetStringValue(valueJSON);
            item.label  = cJSON_GetStringValue(labelJSON);
            vd -> choices -> push_back(item);
        }
    }
    else
    {
        return 0;
    }

    if (cJSON_HasObjectItem(json, "default"))
    {
        cJSON *defaultJSON = cJSON_GetObjectItem(json, "default");
        if (type == "boolean")
        {
            if (!cJSON_IsBool(defaultJSON))
            {
                *errorString = "The variable is a boolean variable, but has a non-boolean default value. The acceptable values are either \"true\" or \"false\", without quotes.";
                return 0;
            }

            if (cJSON_IsTrue(defaultJSON))
            {
                vd -> defaultValue = "true";
            }
        }
        else
        {
            if (!cJSON_IsString(defaultJSON))
            {
                *errorString = "The variable has a non-string default value. Did you forget the quote marks?";
                return 0;
            }
            else
            {
                vd -> defaultValue = cJSON_GetStringValue(defaultJSON);
            }
        }
    }

    vd -> label = new std::string(
        cJSON_GetStringValue(labelJSON)
    );
    vd -> name  = new std::string(
        cJSON_GetStringValue(nameJSON)
    );
    return vd;
}
