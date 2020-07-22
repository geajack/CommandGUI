#include "VariableEntryWidgetGTK.h"

void VariableEntryWidgetGTK::onInternalDataChange()
{
    onChangeSignal.emit();
}

/**************************************************************
 *
 * StringVariableEntryWidgetGTK
 *
 * ************************************************************/

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

Gtk::Widget *StringVariableEntryWidgetGTK::getWidget()
{
    return &widget;
}

/**************************************************************
 *
 * BooleanVariableEntryWidgetGTK
 *
 * ************************************************************/

BooleanVariableEntryWidgetGTK::BooleanVariableEntryWidgetGTK(bool initialValue)
{
    if (initialValue)
    {
        checkbox.set_active(true);
    }
    else
    {
        checkbox.set_active(false);
    }

    checkbox.signal_toggled().connect(
        sigc::mem_fun(*this, &VariableEntryWidgetGTK::onInternalDataChange)
    );
}

std::string BooleanVariableEntryWidgetGTK::getStringValue()
{
    if (checkbox.get_active())
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

Gtk::Widget *BooleanVariableEntryWidgetGTK::getWidget()
{
    return &checkbox;
}