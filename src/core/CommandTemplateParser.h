#pragma once

#include <map>
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
        std::map<std::string, std::string>* variableMap;
        ExceptionCode exceptionCode;
        std::string errorMessage;
        
        int mode;
        int depth;
        int depthSinceHidden;
        std::string variableName;
        
        void step(char qc);
        void textStep(char qc);
        void variableNameStep(char qc);
        void escapedStep(char qc);
        void lastStep();
        
        void print(std::string s);
        bool isVariableTrueOrNonEmpty(std::string name);
        
    public:
        CommandTemplateParser(CommandDescriptor* commandDescriptor);
        void parse();
        void addVariable(const std::string *name, const std::string *value);
        ExceptionCode getError();
        std::string getErrorMessage();
        std::string getResult();
};
