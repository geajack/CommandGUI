#pragma once

#include <string>
#include <gtkmm.h>
#include "../../core/MultipleChoiceItem.h"
class VariableEntryWidgetGTK
{
    public:
        sigc::signal<void> onChangeSignal;
        void onInternalDataChange();
        virtual Gtk::Widget* getWidget() = 0;
        virtual std::string getStringValue() = 0;
};

class StringVariableEntryWidgetGTK : public VariableEntryWidgetGTK
{
        Gtk::Entry widget;
    public:
        StringVariableEntryWidgetGTK(std::string initialValue);
        std::string getStringValue();
        Gtk::Widget* getWidget();
};

class BooleanVariableEntryWidgetGTK : public VariableEntryWidgetGTK
{
        Gtk::CheckButton checkbox;
    public:
        BooleanVariableEntryWidgetGTK(bool initialValue);
        std::string getStringValue();
        Gtk::Widget* getWidget();
};

class FolderVariableEntryWidgetGTK : public VariableEntryWidgetGTK
{
        Gtk::FileChooserButton button;
    public:
        FolderVariableEntryWidgetGTK(std::string initialValue);
        std::string getStringValue();
        Gtk::Widget* getWidget();
};

class FileVariableEntryWidgetGTK : public VariableEntryWidgetGTK
{
        Gtk::FileChooserButton button;
    public:
        FileVariableEntryWidgetGTK(std::string initialValue);
        std::string getStringValue();
        Gtk::Widget* getWidget();
};

class MultipleChoiceVariableEntryWidgetGTK : public VariableEntryWidgetGTK
{
        Gtk::ComboBoxText comboBox;
    public:
        MultipleChoiceVariableEntryWidgetGTK(std::vector<MultipleChoiceItem> *options, std::string initialValue);
        std::string getStringValue();
        Gtk::Widget* getWidget();
};