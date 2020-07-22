#include <string>
#include <iostream>
#include "CommandDescriptor.h"
#include "CommandTemplateParser.h"
#include "Exceptions.h"

int main(int argc, const char** argv)
{
    ExceptionCode statusCode;
    std::string errorMessage;

    std::string *filePath = new std::string(argv[1]);

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