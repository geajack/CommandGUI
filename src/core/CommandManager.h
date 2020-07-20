#pragma once

#include "CommandDescriptor.h"
#include <stdio.h>
#include <QFileInfoList>
#include "Exceptions.h"

class CommandManager
{
        std::vector<CommandHeader>* commandHeaders;
        QFileInfoList* fileInfoList;
        ExceptionCode exceptionCode;
        std::string errorMessage;
        
    public:
        CommandManager();
        void initialize();
        std::vector<CommandHeader>* getHeaders();
        CommandDescriptor* getCommandDescriptor(int id);
        ExceptionCode getError();
        std::string getErrorMessage();
        int commandCount;
};