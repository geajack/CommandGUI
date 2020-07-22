#include <string>
#include <cstdlib>
#include <iostream>
#include "CommandDescriptor.h"
#include "CommandTemplateParser.h"
#include "Exceptions.h"

int main(int argc, const char** argv)
{
    std::string command = argv[1];
    std::string filePath = argv[2];

    ExceptionCode statusCode;
    std::string errorMessage;

    CommandDescriptor *descriptor = CommandDescriptor::FromJSON(
        &filePath,
        &statusCode,
        &errorMessage
    );

    if (statusCode == X_OKAY)
    {
        if (command == "validate")
        {
            return 0;
        }
        else if (command == "parse")
        {
            CommandTemplateParser parser(descriptor);

            for (VariableDescriptor *variable : *descriptor->getVariableList())
            {
                auto name = variable->name;
                const char *result = std::getenv(name->c_str());

                std::string value;
                if (result == NULL)
                {
                    value = variable->defaultValue;
                }
                else
                {
                    value = std::string(result);
                }

                parser.addVariable(name, &value);
            }

            parser.parse();
            if (parser.getError() != X_OKAY)
            {
                std::cout << parser.getErrorMessage() << "\n";
                return 1;
            }
            else
            {
                std::cout << parser.getResult() << "\n";
                return 0;
            }
        }
    }
    else
    {
        std::cout << errorMessage << "\n";
        return 1;
    }
}