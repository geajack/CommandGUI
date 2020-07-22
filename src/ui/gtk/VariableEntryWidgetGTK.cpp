#include "VariableEntryWidgetGTK.h"

void VariableEntryWidgetGTK::onInternalDataChange()
{
    onChangeSignal.emit();
}

StringVariableEntryWidgetGTK::StringVariableEntryWidgetGTK(std::string initialValue)
{
    widget.set_text(initialValue);
    widget.signal_changed().connect(
        sigc::mem_fun(*this, &VariableEntryWidgetGTK::onInternalDataChange)
    );
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