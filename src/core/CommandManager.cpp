#include <string.h>
#include "CommandTemplateParser.h"
#include "CommandDescriptor.h"
#include "CommandManager.h"
#include "cJSON.h"

CommandManager::CommandManager()
{
    exceptionCode = X_OKAY;
    fileInfoList = new std::vector<std::filesystem::path>();
}

void CommandManager::initialize(std::string homeDirectoryPath)
{
    using namespace std::filesystem;
    
    directory_entry directory = directory_entry(homeDirectoryPath);

    if (directory.exists() && directory.is_directory())
    {
        for (directory_entry entry : directory_iterator(directory))
        {
            path filePath = entry.path();
            if (filePath.extension().string() == ".json")
            {
                fileInfoList->push_back(filePath);
            }
        }
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
        header.name = new std::string(fileInfoList->at(i).stem().string());
        list->push_back(header);
    }
    return list;
}

CommandDescriptor* CommandManager::getCommandDescriptor(int id)
{
    exceptionCode = X_OKAY;
    
    std::string filePath = fileInfoList->at(id).string();
    CommandDescriptor* cd = CommandDescriptor::FromJSON(&filePath, &exceptionCode, &errorMessage);
    if (cd == 0)
    {
        errorMessage = "There was a problem with the file " + (fileInfoList -> at(id).string()) + ".\n\n" + errorMessage;
        return 0;
    }

    CommandTemplateParser parser = CommandTemplateParser(cd);
    ExceptionCode status = parser.validate();
    
    if (status == X_OKAY)
    {
        return cd;
    }
    else
    {
        exceptionCode = parser.getError();
        errorMessage = parser.getErrorMessage();
        return NULL;
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
