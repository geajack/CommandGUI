#include <QList>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>
#include "CommandDescriptor.h"
#include "Exceptions.h"

CommandDescriptor* CommandDescriptor::FromJSON(QJsonObject qJsonObj, std::string* errorString)
{
    CommandDescriptor* cd = new CommandDescriptor();

    if (! qJsonObj.contains("templateString") )
    {
        *errorString = "There was no template string.";
        return 0;
    }

    if (! qJsonObj.value("templateString").isString())
    {
        *errorString = "The template string value was not a string.";
        return 0;
    }

    if (qJsonObj.contains("variables") && (! qJsonObj.value("variables").isArray()))
    {
        *errorString = "The variables array was not an array.";
        return 0;
    }

    cd -> templateString = qJsonObj.value("templateString").toString().toStdString();
    cd -> variableMap = new QMap<std::string, VariableDescriptor*>;
    cd -> variableList = new QList<VariableDescriptor*>;
    QJsonArray variables = qJsonObj.value("variables").toArray();

    VariableDescriptor* vd;
    std::string variableErrorString;
    for (int i = 0; i < variables.count(); i++)
    {
        vd = VariableDescriptor::FromJSON(variables.at(i).toObject(), &variableErrorString);

        if (vd == 0)
        {
            *errorString = "There was a problem parsing variable number " + std::to_string(i + 1) + " of your variable list. ";
            *errorString += variableErrorString;
            return 0;
        }

        cd -> variableMap -> insert(*(vd -> name), vd);
        cd -> variableList -> append(vd);
    }

    return cd;
}

QList<VariableDescriptor*>* CommandDescriptor::getVariableList()
{
    return variableList;
}

QMap<std::string, VariableDescriptor*>* CommandDescriptor::getVariableMap()
{
    return variableMap;
}

VariableDescriptor* VariableDescriptor::FromJSON(QJsonObject qJsonObj, std::string* errorString)
{
    VariableDescriptor* vd = new VariableDescriptor();

    if (! qJsonObj.contains("type"))
    {
        *errorString = "The variable had no \"type\" attribute.";
        return 0;
    }

    if (! qJsonObj.value("type").isString())
    {
        *errorString = "The variable's \"type\" attribute was not a string.";
        return 0;
    }

    if (! qJsonObj.contains("name"))
    {
        *errorString = "The variable had no \"name\" attribute.";
        return 0;
    }

    if (! qJsonObj.value("name").isString())
    {
        *errorString = "The variable's \"name\" attribute was not a string.";
        return 0;
    }

    if (! qJsonObj.contains("label"))
    {
        *errorString = "The variable had no \"label\" attribute.";
        return 0;
    }

    if (! qJsonObj.value("label").isString())
    {
        *errorString = "The variable's \"label\" attribute was not a string.";
        return 0;
    }

    QString type = qJsonObj.value("type").toString();

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
        if (! qJsonObj.contains("choices"))
        {
            *errorString = "The variable is a multiple choice type variable, but has no \"choices\" array.";
            return 0;
        }

        if (! qJsonObj.value("choices").isArray())
        {
            *errorString = "The variable is a multiple choice type variable, and the \"choices\" attribute is not an array.";
            return 0;
        }

        vd -> type = VariableDescriptor::TYPE_MULTIPLE_CHOICE;
        vd -> defaultValue = "";
        vd -> choices = new QList<VariableDescriptor::MultipleChoiceItem>;
        QJsonArray choices = qJsonObj.value("choices").toArray();
        QJsonObject choiceJsonObj;
        VariableDescriptor::MultipleChoiceItem item;
        for (int i = 0; i < choices.count(); i++)
        {
            choiceJsonObj = choices.at(i).toObject();

            if (! choiceJsonObj.contains("value"))
            {
                *errorString = "A member of the \"choices\" array has no \"value\" attribute.";
                return 0;
            }

            if (! choiceJsonObj.value("value").isString())
            {
                *errorString = "A member of the \"choices\" array has a non-string \"value\" attribute.";
                return 0;
            }

            if (! choiceJsonObj.contains("label"))
            {
                *errorString = "A member of the \"choices\" array has no \"label\" attribute.";
                return 0;
            }

            if (! choiceJsonObj.value("label").isString())
            {
                *errorString = "A member of the \"choices\" array has a non-string \"label\" attribute.";
                return 0;
            }

            item.value = choiceJsonObj.value("value").toString();
            item.label  = choiceJsonObj.value("label").toString();
            vd -> choices -> append(item);
        }
    }
    else
    {
        return 0;
    }

    if (qJsonObj.contains("default"))
    {
        if (type == "boolean")
        {
            if (! qJsonObj.value("default").isBool())
            {
                *errorString = "The variable is a boolean variable, but has a non-boolean default value. The acceptable values are either \"true\" or \"false\", without quotes.";
                return 0;
            }

            if (qJsonObj.value("default").toBool())
            {
                vd -> defaultValue = "true";
            }
        }
        else
        {
            if (! qJsonObj.value("default").isString())
            {
                *errorString = "The variable has a non-string default value. Did you forget the quote marks?";
                return 0;
            }
            else
            {
                vd -> defaultValue = qJsonObj.value("default").toString().toStdString();
            }
        }
    }

    vd -> label = new std::string(
        qJsonObj.value("label").toString().toStdString()
    );
    vd -> name  = new std::string(
        qJsonObj.value("name").toString().toStdString()
    );
    return vd;
}
