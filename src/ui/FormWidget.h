#pragma once

#include <string.h>
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QMap>
#include <QList>

struct DropDownListEntry
{
    QString value;
    QString label;
};

struct FormFieldDescriptor
{
    QWidget* field;
    int      type;
};

class FormWidget : public QWidget
{
    Q_OBJECT
    QFormLayout* layout;
    QMap<QString, FormFieldDescriptor*>* formMap;
    
    public:
    
        static const int TYPE_TEXT = 0;
        static const int TYPE_BOOLEAN = 1;
        static const int TYPE_FILE = 2;
        static const int TYPE_DROPDOWN = 3;
    
        explicit FormWidget(QWidget* parent = 0);
        void addTextItem(QString name, QString label, QString def);
        void addBooleanItem(QString name, QString label, bool def);
        void addFileItem(QString name, QString label, QString def, int type);
        void addDropDownItem(QString name, QString label, QString def, QList<DropDownListEntry>* entries);
        QMap<std::string, std::string>* getFormData();
        ~FormWidget();
        
    signals:
        
        void formChanged();
};
