#include "VariableEntryWidgetGTK.h"

StringVariableEntryWidgetGTK::StringVariableEntryWidgetGTK(std::string initialValue)
{
    widget.set_text(initialValue);
}

std::string StringVariableEntryWidgetGTK::getStringValue()
{
    return widget.get_text();
}

Gtk::Entry *StringVariableEntryWidgetGTK::getWidget()
{
    return &widget;
}

BooleanVariableEntryWidgetGTK::BooleanVariableEntryWidgetGTK(bool initialValue) : StringVariableEntryWidgetGTK("")
{
    if (initialValue)
    {
        getWidget()->set_text("true");
    }
    else
    {
        getWidget()->set_text("false");
    }
}