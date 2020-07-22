#include <string>
#include <gtkmm.h>

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

typedef StringVariableEntryWidgetGTK MultipleChoiceVariableEntryWidgetGTK;
typedef StringVariableEntryWidgetGTK FileVariableEntryWidgetGTK;
typedef StringVariableEntryWidgetGTK FolderVariableEntryWidgetGTK;