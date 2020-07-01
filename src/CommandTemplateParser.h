#pragma once

#include <QMap>
#include "CommandDescriptor.h"
#include "Exceptions.h"

class CommandTemplateParser
{
        static const int TEXT = 0;
        static const int VARIABLE_NAME = 1;
        static const int ESCAPED = 2;
        
        std::string input;
        std::string output;
        CommandDescriptor* commandDescriptor;
        QMap<std::string, std::string>* variableMap;
        ExceptionCode exceptionCode;
        std::string errorMessage;
        
        int mode;
        int depth;
        int depthSinceHidden;
        std::string variableName;
        
        void step(QChar qc);
        void textStep(QChar qc);
        void variableNameStep(QChar qc);
        void escapedStep(QChar qc);
        void lastStep();
        
        void print(std::string s);
        bool isVariableTrueOrNonEmpty(std::string name);
        
    public:
        CommandTemplateParser(CommandDescriptor* commandDescriptor, QMap<std::string, std::string>* variableMap);
        void parse();
        ExceptionCode getError();
        std::string getErrorMessage();
        std::string getResult();
};
