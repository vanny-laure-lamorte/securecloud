#include "MainWindow.h"
#include "NotConnectedLayout.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>

MainWindow::MainWindow(ClientService* service, QWidget *parent, TranslationManager* tm)
    : QWidget(parent), service_(service), translationManager_(tm)
{
    // Get style file
    QFile file(":/styles/secure-cloud-style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString style = file.readAll();
        this->setStyleSheet(style);
    } else {
        qWarning("Can't reach secure-cloud-style.qss");
    };

    setWindowTitle("SecureCloud");
    resize(1280, 720);

    layoutContainer = new NotConnectedLayout(service_, this, translationManager_);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(layoutContainer);
}