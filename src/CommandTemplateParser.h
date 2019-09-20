#pragma once

#include <QMap>
#include <QString>
#include "CommandDescriptor.h"
#include "Exceptions.h"

class CommandTemplateParser
{
        static const int TEXT = 0;
        static const int VARIABLE_NAME = 1;
        static const int ESCAPED = 2;
        
        QString input;
        QString output;
        CommandDescriptor* commandDescriptor;
        QMap<QString, QString>* variableMap;
        ExceptionCode exceptionCode;
        QString errorMessage;
        
        int mode;
        int depth;
        int depthSinceHidden;
        QString variableName;
        
        void step(QChar qc);
        void textStep(QChar qc);
        void variableNameStep(QChar qc);
        void escapedStep(QChar qc);
        void lastStep();
        
        void print(QString s);
        bool isVariableTrueOrNonEmpty(QString name);
        
    public:
        CommandTemplateParser(CommandDescriptor* commandDescriptor, QMap<QString, QString>* variableMap);
        void parse();
        ExceptionCode getError();
        QString getErrorMessage();
        QString getResult();
};
