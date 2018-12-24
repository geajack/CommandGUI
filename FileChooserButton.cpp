#include <QFileDialog>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include "FileChooserButton.h"

FileChooserButton::FileChooserButton(QString label, QWidget* parent = 0) : QWidget(parent)
{
    button = new QPushButton(this);
    dropLabel = new QLabel(this);
    filePathLabel = new QLabel(this);
    setType(FileChooserButton::FILE);
    dropLabel->setStyleSheet("font-weight: bold;");
    dropLabel->setVisible(false);
    button->setText(label);
    button->setMaximumWidth(100);
    setAcceptDrops(true);
    setMinimumHeight(button->height());
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(dropLabel);
    layout->addWidget(button);
    layout->addWidget(filePathLabel);
    layout->addStretch(0);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    connect(button, SIGNAL(clicked()), this, SLOT(openDialog()));
}

void FileChooserButton::dragEnterEvent(QDragEnterEvent* e)
{
    dropLabel->setVisible(true);
    button->setVisible(false);
    filePathLabel->setVisible(false);
    e->acceptProposedAction();
}

void FileChooserButton::dragLeaveEvent(QDragLeaveEvent* e)
{
    dropLabel->setVisible(false);
    button->setVisible(true);
    filePathLabel->setVisible(true);
}

void FileChooserButton::dropEvent(QDropEvent* e)
{
    dropLabel->setVisible(false);
    button->setVisible(true);
    filePathLabel->setVisible(true);
    if (e->mimeData()->hasUrls())
    {
        int length = e->mimeData()->urls().length();
        QUrl url;
        for (int i = 0; i < length; i++)
        {
            url = e->mimeData()->urls().at(i);
            if (url.isLocalFile())
            {
                QFileInfo fileInfo = QFileInfo(url.toLocalFile());
                if (this->type == FileChooserButton::FILE)
                {
                    if (fileInfo.isFile())
                    {
                        setPath(url.toLocalFile());
                        emit changed();
                        break;
                    }
                }
                else
                {
                    if (fileInfo.isDir())
                    {
                        setPath(url.toLocalFile());
                        emit changed();
                        break;
                    }
                }
            }
        }
    }
}

void FileChooserButton::openDialog()
{
    if (type == FileChooserButton::FILE)
    {
        filePath = QFileDialog::getOpenFileName(this, "", defaultDirectoryPath);
    }
    else
    {
        filePath = QFileDialog::getExistingDirectory(this, "", defaultDirectoryPath);
    }
    setPath(filePath);
    emit changed();
}

void FileChooserButton::setType(int type)
{
    this->type = type;  
    
    if (type == FileChooserButton::FILE)
    {
        dropLabel->setText("Drop file here");
        filePathLabel->setText("Click Browse or drag and drop a file.");
    }
    else
    {
        dropLabel->setText("Drop folder here");
        filePathLabel->setText("Click Browse or drag and drop a folder.");
    }
}

void FileChooserButton::setPath(QString path)
{
    this->filePath = path;
    
    if (! path.isEmpty())
    {
        filePathLabel->setText(path);
    }
    else
    {
        if (type == FileChooserButton::FILE)
        {
            filePathLabel->setText("Click Browse or drag and drop a file.");
        }
        else
        {
            filePathLabel->setText("Click Browse or drag and drop a folder.");
        }
        
    }
}

void FileChooserButton::setDefaultPath(QString path)
{
    this->defaultDirectoryPath = path;
}

QString FileChooserButton::getFilePath()
{
    return filePath;
}

