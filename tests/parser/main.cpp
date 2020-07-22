#include <string>
#include <iostream>
#include "CommandDescriptor.h"
#include "CommandTemplateParser.h"
#include "Exceptions.h"

int main()
{
    ExceptionCode statusCode;
    std::string errorMessage;

    std::string *filePath = new std::string("/home/jack/Code/CommandGUI/Codebase/tests/parser/cases/invalid.json");

    CommandDescriptor::FromJSON(
        filePath,
        &statusCode,
        &errorMessage
    );

    std::cout << statusCode << "\n";
    std::cout << errorMessage << "\n";

    delete filePath;

    return 0;
}