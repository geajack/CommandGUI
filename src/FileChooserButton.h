#pragma once

#include <QPushButton>
#include <QString>
#include <QLabel>


class FileChooserButton : public QWidget
{
    Q_OBJECT
        
    QString filePath;
    QString defaultDirectoryPath;
    
    QPushButton* button;
    QLabel* filePathLabel;
    QLabel* dropLabel;
    
    void dragEnterEvent(QDragEnterEvent* e);
    void dragLeaveEvent(QDragLeaveEvent* e);
    void dropEvent(QDropEvent* e);
        
    public:
        static const int FILE   = 0;
        static const int FOLDER = 1;
        int type;
    
        FileChooserButton(QString label, QWidget* parent);
        QString getFilePath();
        void setType(int type);
        void setPath(QString path);
        void setDefaultPath(QString path);
        
    signals:
        void changed();
        
    public slots:
        void openDialog();
};
