#include <string.h>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QDirIterator>
#include <QJsonDocument>
#include <QStringList>
#include <QJsonObject>
#include "CommandDescriptor.h"
#include "CommandManager.h"
#include <QMap>
#include <QFileInfoList>
#include <QList>
#include <QFileInfo>
#include <QJsonParseError>
#include <QCoreApplication>

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

QList<CommandHeader>* CommandManager::getHeaders()
{
    QList<CommandHeader>* list = new QList<CommandHeader>;
    CommandHeader header;
    
    for (int i = 0; i < fileInfoList->length(); i++)
    {
        header.id   = i;
        header.name = new std::string(fileInfoList->at(i).baseName().toStdString());
        list->append(header);
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
        file.setFileName((fileInfoList -> at(id).filePath()));
        file.open(QIODevice::ReadOnly);    
        QByteArray fileContents = file.readAll();
        
        if (file.error())
        {
            exceptionCode = X_FILE_UNREADABLE;
            errorMessage  = "There was a problem with the file " + (fileInfoList -> at(id).filePath()).toStdString() + ". ";
            errorMessage  += "Is it corrupted or missing?";
            throw 0;
        }
        
        QJsonParseError jsonParseError;
        QJsonDocument qJsonDoc = QJsonDocument::fromJson(fileContents, &jsonParseError);
        
        if (jsonParseError.error != QJsonParseError::NoError)
        {
            exceptionCode = X_BAD_JSON_SYNTAX;
            setErrorMessage(jsonParseError, fileContents);
            errorMessage = "There was a problem with the file " + (fileInfoList -> at(id).filePath()).toStdString() + ".\n\n" + errorMessage;
            throw 0;
        }
        
        cd = CommandDescriptor::FromJSON(qJsonDoc.object(), &errorMessage);
        
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

void CommandManager::setErrorMessage(QJsonParseError jsonParseError, QByteArray fileContents)
{
    int lineNumber = 0;
    int i = 0;
    while (i <= jsonParseError.offset)
    {
        if (fileContents.at(i) == '\n')
        {
            lineNumber++;
        }
        i++;
    }
    
    errorMessage = "There was a problem parsing your JSON around line " + std::to_string(lineNumber) + ".\n";
    
    switch(jsonParseError.error)
    {
        case QJsonParseError::UnterminatedObject:
            errorMessage += "The parser says it found an object without an ending curly brace.";
        break;
        
        case QJsonParseError::MissingNameSeparator:
            errorMessage += "The parser says you forgot to put in a comma between array items.";
        break;
        
        case QJsonParseError::UnterminatedArray:
            errorMessage += "The parser says it found an array without an ending square breacket.";
        break;
        
        case QJsonParseError::MissingValueSeparator:
            errorMessage += "The parser says you forgot to put a colon between a key and a value.";
        break;
        
        case QJsonParseError::IllegalValue:
            errorMessage += "The parser says it found an illegal value.";
        break;
        
        case QJsonParseError::TerminationByNumber:
            errorMessage += "The parser says the file ended with a number.";
        break;
        
        case QJsonParseError::IllegalNumber:
            errorMessage += "The parser says it found an illegal number.";
        break;
        
        case QJsonParseError::IllegalEscapeSequence:
            errorMessage += "The parser says it found an illegal escape (backslash) sequence.";
        break;
        
        case QJsonParseError::IllegalUTF8String:
            errorMessage += "The parser says it found an illegal UTF-8 string.";
        break;
        
        case QJsonParseError::UnterminatedString:
            errorMessage += "The parser says it found a string without an ending quote.";
        break;
        
        case QJsonParseError::MissingObject:
            errorMessage += "The parser says there was supposed to be a JSON object at this point.";
        break;
        
        case QJsonParseError::DeepNesting:
            errorMessage += "The parser says the JSON file was too deeply nested.";
        break;
        
        case QJsonParseError::DocumentTooLarge:
            errorMessage += "The parser says the JSON document was too big.";
        break;
        
        case QJsonParseError::GarbageAtEnd:
            errorMessage += "The parser says there was garbage data at the end of the file.";
        break;     
    }
}

ExceptionCode CommandManager::getError()
{
    return exceptionCode;
}

std::string CommandManager::getErrorMessage()
{
    return errorMessage;
}
