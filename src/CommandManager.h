#pragma once

#include "CommandDescriptor.h"
#include <stdio.h>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfoList>
#include <QFileInfo>
#include <QByteArray>
#include <QJsonParseError>
#include "Exceptions.h"

class CommandManager
{
        QList<CommandHeader>* commandHeaders;
        QFileInfoList* fileInfoList;
        ExceptionCode exceptionCode;
        QString errorMessage;
        void setErrorMessage(QJsonParseError jsonParseError, QByteArray fileContents);
        
    public:
        CommandManager();
        void initialize();
        QList<CommandHeader>* getHeaders();
        CommandDescriptor* getCommandDescriptor(int id);
        ExceptionCode getError();
        QString getErrorMessage();
        int commandCount;
};