#include "CommandPage.h"

CommandPage::CommandPage()
{
    button.set_label("Back");
    button.signal_clicked().connect(
        sigc::mem_fun(*this, &CommandPage::onClickBack)
    );

    add(label);
    add(button);
}

void CommandPage::loadCommandDescriptor(CommandDescriptor *descriptor)
{
    auto name = descriptor->name;
    auto nVariables = descriptor->variableList->size();

    label.set_text(name);
}

void CommandPage::onClickBack()
{
    signal_clicked_back.emit();
}