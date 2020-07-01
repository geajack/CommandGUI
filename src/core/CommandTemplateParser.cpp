#include <QMap>
#include "CommandDescriptor.h"
#include "CommandTemplateParser.h"
#include "Exceptions.h"

CommandTemplateParser::CommandTemplateParser(CommandDescriptor *commandDescriptor, QMap<std::string, std::string>* variableMap)
{
    this -> commandDescriptor = commandDescriptor;
    this -> variableMap = variableMap;
    this -> input = (commandDescriptor -> templateString);
}

inline void CommandTemplateParser::step(QChar qc)
{
    switch (mode)
    {
        case TEXT:
            textStep(qc);
        break;
        
        case VARIABLE_NAME:
            variableNameStep(qc);
        break;
        
        case ESCAPED:
            escapedStep(qc);
        break;
    }
}

inline void CommandTemplateParser::textStep(QChar qc)
{
    char c = qc.toLatin1();
    
    bool printVariable  = false; // If we just finished parsing a variable name, print it.
    bool printCharacter = false;
    
    switch (c)
    {
        case '$':
            mode = ESCAPED;
            printVariable = true;
        break;
        
        case '%':
            mode = VARIABLE_NAME;
            printVariable = true;
        break;
        
        case '[':
            if (variableName.length() > 0)
            {
                depth++;
                if (isVariableTrueOrNonEmpty(variableName))
                {
                    if (depthSinceHidden > 0)
                    {
                        depthSinceHidden++;
                    }
                }
                else
                {
                    depthSinceHidden++;
                }
                variableName = "";
            }
            else
            {
                exceptionCode = X_BAD_TEMPLATE;
                throw "You started a conditional (square bracket) block without putting a variable in front of it.";
            }
        break;
        
        case ']':
            depth--;
            if (depth < 0)
            {
                exceptionCode = X_BAD_TEMPLATE;
                throw "Your square brackets are mis-matched. I found one too many close brackets.";
            }
            if (depthSinceHidden > 0)
            {
                depthSinceHidden--;
            }
            else
            {
                printVariable = true;
            }
        break;
            
        default:
            printCharacter = true;
            printVariable = true;
        break;
    }
    
    
    if (! variableName.empty())
    {
        if (printVariable)
        {
            if (commandDescriptor -> getVariableMap() -> value(variableName) -> type == VariableDescriptor::TYPE_BOOLEAN)
            {
                exceptionCode = X_BAD_TEMPLATE;
                throw "The template string references a variable called \"" + variableName + "\" which isn't in the variables list.";
            }
            else
            {
                print(variableMap -> value(variableName));
            }
        }
        variableName = "";
    }
    
    
    if (printCharacter)
    {
        print({c});
    }
}

inline bool CommandTemplateParser::isVariableTrueOrNonEmpty(std::string name)
{
    VariableDescriptor* vd = commandDescriptor -> getVariableMap() -> value(name);
    if (vd -> type == VariableDescriptor::TYPE_BOOLEAN)
    {
        return (variableMap -> value(name)) == "true";
    }
    else
    {
        return ! (variableMap -> value(name)).empty();
    }
}

inline void CommandTemplateParser::variableNameStep(QChar qc)
{
    char c = qc.toLatin1();
    
    if (c == '%')
    {
        mode = TEXT;
    }
    else if (qc.isLetterOrNumber())
    {
        variableName += c;
    }
    else
    {
        exceptionCode = X_BAD_TEMPLATE;
        throw "Variable names can only contain letters and numbers. I found a variable name containing a \"" + QString(qc) + "\".";
    }
}

inline void CommandTemplateParser::escapedStep(QChar qc)
{
    char c = qc.toLatin1();
    
    switch (c)
    {
        case '$':
        case '%':
        case '[':
        case ']':
            mode = TEXT;
            print({c});
        break;
            
        default:
            exceptionCode = X_BAD_TEMPLATE;
            throw "The dollar sign ($) is to be used as an escape character in template strings. It should only be used to escape the characters %, [, ] or itself.";
        break;
    }
}

void CommandTemplateParser::lastStep()
{
    switch (mode)
    {
        case TEXT:
            if (! variableName.empty())
            {
                if (commandDescriptor -> getVariableMap() -> value(variableName) -> type == VariableDescriptor::TYPE_BOOLEAN)
                {
                    exceptionCode = X_BAD_TEMPLATE;
                    throw "The template string references a variable called \"" + variableName + "\" which isn't in the variables list.";
                }
                else
                {
                    print(variableMap -> value(variableName));
                }
                variableName = "";
            }
        break;
        
        case VARIABLE_NAME:
            exceptionCode = X_BAD_TEMPLATE;
            throw "The template string ends in the middle of a variable name.";
        break;
        
        case ESCAPED:
            exceptionCode = X_BAD_TEMPLATE;
            throw "The dollars sign is to be used as an escape character in template strings. I found one at the end of the string. To insert an actual dollar sign as text, use a double dollar sign $$.";
        break;
    }
}

void CommandTemplateParser::print(std::string s)
{
    if (depthSinceHidden == 0)
    {
        output += s;
    }
}

void CommandTemplateParser::parse()
{    
    exceptionCode = X_OKAY;    
    
    mode = TEXT;
    depth = 0;
    depthSinceHidden = 0;
    output = "";
    variableName = "";
    
    int n = input.length();
    try
    {
        for (int i = 0; i < n; i++)
        {
            step(input.at(i));
        }
        
        lastStep();
    }
    catch (QString msg)
    {
        errorMessage = "There was a problem parsing the template string. ";
        errorMessage += msg.toStdString();
    }
}

ExceptionCode CommandTemplateParser::getError()
{
    return exceptionCode;
}

std::string CommandTemplateParser::getErrorMessage()
{
    return errorMessage;
}

std::string CommandTemplateParser::getResult()
{
    return output;
}