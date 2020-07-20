#pragma once

#include "CommandDescriptor.h"
#include <stdio.h>
#include <filesystem>
#include "Exceptions.h"

class CommandManager
{
        std::vector<CommandHeader>* commandHeaders;
        std::vector<std::filesystem::path> *fileInfoList;
        ExceptionCode exceptionCode;
        std::string errorMessage;
        
    public:
        CommandManager();
        void initialize(std::string homeDirectoryPath);
        std::vector<CommandHeader>* getHeaders();
        CommandDescriptor* getCommandDescriptor(int id);
        ExceptionCode getError();
        std::string getErrorMessage();
        int commandCount;
};