#pragma once

#include "CommandDescriptor.h"
#include <stdio.h>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QDirIterator>
#include <QFileInfoList>
#include <QFileInfo>
#include <QByteArray>
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