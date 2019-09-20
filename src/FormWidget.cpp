#include <QString>
#include <QMap>
#include <QFormLayout>
#include <QLineEdit>
#include <QVariant>
#include <QCheckBox>
#include <QComboBox>
#include <QDataStream>
#include <QDebug>
#include <QSizePolicy>
#include "FormWidget.h"
#include "FileChooserButton.h"

FormWidget::FormWidget(QWidget* parent) : QWidget(parent)
{
    formMap = new QMap<QString, FormFieldDescriptor*>;
    layout = new QFormLayout();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setLayout(layout);
}

void FormWidget::addTextItem(QString name, QString label, QString defaultValue)
{
    QLineEdit* inputField = new QLineEdit(this);
    inputField -> setText(defaultValue);
    connect(inputField, SIGNAL(textChanged(QString)), this, SIGNAL(formChanged()));
    FormFieldDescriptor* descriptor = (FormFieldDescriptor*) malloc(sizeof(FormFieldDescriptor));
    descriptor -> type  = TYPE_TEXT;
    descriptor -> field = inputField;
    layout  -> addRow(label, inputField);
    formMap -> insert(name, descriptor);
}

void FormWidget::addBooleanItem(QString name, QString label, bool def)
{
    QCheckBox* inputField = new QCheckBox();
    connect(inputField, SIGNAL(stateChanged(int)), this, SIGNAL(formChanged()));
    FormFieldDescriptor* descriptor = (FormFieldDescriptor*) malloc(sizeof(FormFieldDescriptor));
    
    if (def)
    {
        inputField -> setChecked(true);
    }
    
    descriptor -> type  = TYPE_BOOLEAN;
    descriptor -> field = inputField;
    layout  -> addRow(label, inputField);
    formMap -> insert(name, descriptor);
}

void FormWidget::addFileItem(QString name, QString label, QString defaultValue, int type)
{
    FileChooserButton* inputField = new FileChooserButton("Browse...", this);
    inputField->setDefaultPath(defaultValue);
    inputField->setPath(defaultValue);
    inputField->setType(type);
    connect(inputField, SIGNAL(changed()), this, SIGNAL(formChanged()));
    FormFieldDescriptor* descriptor = (FormFieldDescriptor*) malloc(sizeof(FormFieldDescriptor));
    descriptor -> type  = TYPE_FILE;
    descriptor -> field = inputField;
    layout  -> addRow(label, inputField);
    formMap -> insert(name, descriptor);
}

void FormWidget::addDropDownItem(QString name, QString label, QString defaultValue, QList<DropDownListEntry>* entries)
{
    QComboBox* inputField = new QComboBox(this);
    connect(inputField, SIGNAL(currentIndexChanged(int)), this, SIGNAL(formChanged()));
    
    inputField -> addItem("---", "");
    int defaultIndex = 0;
    
    for (int i = 0; i < entries -> length(); i++)
    {
        inputField -> addItem(((entries -> at(i)).label), QVariant(((entries -> at(i)).value)));
        if (defaultValue == (entries -> at(i)).value)
        {
            defaultIndex = i + 1;
        }
    }
    
    inputField -> setCurrentIndex(defaultIndex);
    
    FormFieldDescriptor* descriptor = (FormFieldDescriptor*) malloc(sizeof(FormFieldDescriptor));
    descriptor -> type  = TYPE_DROPDOWN;
    descriptor -> field = inputField;
    layout  -> addRow(label, inputField);
    formMap -> insert(name, descriptor);
}

QMap<QString, QString>* FormWidget::getFormData()
{
    QMap<QString, QString>* map = new QMap<QString, QString>;
    
    QMap<QString, FormFieldDescriptor*>::iterator iterator = formMap -> begin();
    
    FormFieldDescriptor* descriptor;
    QString name;
    QString value;
    
    while (iterator != formMap -> end())
    {
        name = iterator.key();
        descriptor = iterator.value();
        value = "";
        
        QWidget* field = descriptor -> field;
        
        switch (descriptor -> type)
        {
            case FormWidget::TYPE_TEXT:
                value = ((QLineEdit*) field) -> text();
            break;
                
            case FormWidget::TYPE_BOOLEAN:
                if (((QCheckBox*) field) -> isChecked())
                    value = "true";
                else
                    value = "false";
            break;
                
            case FormWidget::TYPE_FILE:
                value = ((FileChooserButton*) field)->getFilePath();
            break;
                
            case FormWidget::TYPE_DROPDOWN:
                value = (((QComboBox*) field) -> currentData()).toString();
            break;
        }
        
        map -> insert(name, value);
                
        iterator++;
    }
    
    return map;
}

FormWidget::~FormWidget()
{
    delete formMap;
}
