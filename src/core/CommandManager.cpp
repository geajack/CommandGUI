#include <string.h>
#include <fstream>
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
    
    CommandDescriptor* cd = 0;
    std::ifstream in;
    
    try
    {
        cJSON *json = NULL;
        in = std::ifstream(fileInfoList->at(id).string(), std::ios::in | std::ios::binary);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            json = cJSON_Parse(contents.c_str());
            in.close();
        }
        else
        {
            exceptionCode = X_FILE_UNREADABLE;
            errorMessage  = "There was a problem with the file " + (fileInfoList -> at(id).string()) + ". ";
            errorMessage  += "Is it corrupted or missing?";
            throw 0;
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
            errorMessage = "There was a problem with the file " + (fileInfoList -> at(id).string()) + ".\n\n" + errorMessage;
            throw 0;
        }
    }
    catch (int e)
    {
        if (in.is_open())
        {
            in.close();
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
