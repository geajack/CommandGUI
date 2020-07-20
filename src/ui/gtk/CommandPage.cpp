#include "CommandPage.h"

CommandPage::CommandPage()
{
    label.set_text("Hello, world!");
    add(label);
}

void CommandPage::loadCommandDescriptor(CommandDescriptor *descriptor)
{
    auto name = descriptor->name;
    auto nVariables = descriptor->variableList->size();

    label.set_text(name);
}