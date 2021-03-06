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

/**************************************************************
 *
 * FolderVariableEntryWidgetGTK
 *
 * ************************************************************/

FolderVariableEntryWidgetGTK::FolderVariableEntryWidgetGTK(std::string initialValue)
{
    button.set_current_folder(initialValue);
    button.set_action(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

    button.signal_file_set().connect(
        sigc::mem_fun(*this, &VariableEntryWidgetGTK::onInternalDataChange)
    );
}

std::string FolderVariableEntryWidgetGTK::getStringValue()
{
    return button.get_filename();
}

Gtk::Widget *FolderVariableEntryWidgetGTK::getWidget()
{
    return &button;
}

/**************************************************************
 *
 * FileVariableEntryWidgetGTK
 *
 * ************************************************************/

FileVariableEntryWidgetGTK::FileVariableEntryWidgetGTK(std::string initialValue)
{
    button.set_filename(initialValue);
    button.set_action(Gtk::FILE_CHOOSER_ACTION_SAVE);

    button.signal_file_set().connect(
        sigc::mem_fun(*this, &VariableEntryWidgetGTK::onInternalDataChange)
    );
}

std::string FileVariableEntryWidgetGTK::getStringValue()
{
    return button.get_filename();
}

Gtk::Widget *FileVariableEntryWidgetGTK::getWidget()
{
    return &button;
}

/**************************************************************
 *
 * MultipleChoiceVariableEntryWidgetGTK
 *
 * ************************************************************/

MultipleChoiceVariableEntryWidgetGTK::MultipleChoiceVariableEntryWidgetGTK(std::vector<MultipleChoiceItem> *options, std::string initialValue)
{
    comboBox.append("", "--");
    int index = 1;
    int initialIndex = 0;
    for (MultipleChoiceItem option : *options)
    {
        if (option.value == initialValue)
        {
            initialIndex = index;
        }
        comboBox.append(option.value, option.label);
        index++;
    }
    comboBox.set_active(initialIndex);

    comboBox.signal_changed().connect(
        sigc::mem_fun(*this, &VariableEntryWidgetGTK::onInternalDataChange)
    );
}

std::string MultipleChoiceVariableEntryWidgetGTK::getStringValue()
{
    return comboBox.get_active_id();
}

Gtk::Widget *MultipleChoiceVariableEntryWidgetGTK::getWidget()
{
    return &comboBox;
}