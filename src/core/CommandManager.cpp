#include <string.h>
#include <fstream>
#include <QFile>
#include <QIODevice>
#include <QDirIterator>
#include <QStringList>
#include "CommandDescriptor.h"
#include "CommandManager.h"
#include <QMap>
#include <QFileInfoList>
#include <QFileInfo>
#include <QCoreApplication>
#include "cJSON.h"

CommandManager::CommandManager()
{
    exceptionCode = X_OKAY;
}

void CommandManager::initialize()
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("./commands");
    if (dir.exists())
    {
        fileInfoList = new QFileInfoList(dir.entryInfoList(QStringList() << "*.json", QDir::Files, QDir::Name));
    }
    else
    {
        exceptionCode = X_DIRECTORY_DOES_NOT_EXIST;
        errorMessage = "The /commands directory doesn't exist. Please create a directory called \'commands\' in the same folder as this executable and put your JSON files in it!";
    }
}

std::vector<CommandHeader>* CommandManager::getHeaders()
{
    std::vector<CommandHeader>* list = new std::vector<CommandHeader>;
    CommandHeader header;
    
    for (int i = 0; i < fileInfoList->size(); i++)
    {
        header.id   = i;
        header.name = new std::string(fileInfoList->at(i).baseName().toStdString());
        list->push_back(header);
    }
    return list;
}

CommandDescriptor* CommandManager::getCommandDescriptor(int id)
{
    exceptionCode = X_OKAY;
    
    QFile file;
    CommandDescriptor* cd = 0;
    
    try
    {        
        QString filePath = fileInfoList -> at(id).filePath();
        file.setFileName((filePath));
        file.open(QIODevice::ReadOnly);    
        QByteArray fileContents = file.readAll();
        
        if (file.error())
        {
            exceptionCode = X_FILE_UNREADABLE;
            errorMessage  = "There was a problem with the file " + (fileInfoList -> at(id).filePath()).toStdString() + ". ";
            errorMessage  += "Is it corrupted or missing?";
            throw 0;
        }

        cJSON *json = NULL;
        std::ifstream in(filePath.toStdString(), std::ios::in | std::ios::binary);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            json = cJSON_Parse(contents.c_str());
        }
        
        if (json == NULL)
        {
            exceptionCode = X_BAD_JSON_SYNTAX;
            errorMessage = "The given file contains invalid JSON. " + std::string(cJSON_GetErrorPtr());
            throw 0;
        }
        
        cd = CommandDescriptor::FromJSON(json, &errorMessage);
        
        if (cd == 0)
        {
            exceptionCode = X_BAD_JSON_SEMANTICS;
            errorMessage = "There was a problem with the file " + (fileInfoList -> at(id).filePath()).toStdString() + ".\n\n" + errorMessage;
            throw 0;
        }
    }
    catch (int e)
    {
        if (file.isOpen())
        {
            file.close();
        }
        
        return 0;
    }
    
    return cd;
}

ExceptionCode CommandManager::getError()
{
    return exceptionCode;
}

std::string CommandManager::getErrorMessage()
{
    return errorMessage;
}
