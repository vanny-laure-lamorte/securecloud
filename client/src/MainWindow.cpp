#include "MainWindow.h"
#include "NotConnectedLayout.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
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

    layoutContainer = new NotConnectedLayout(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(layoutContainer);

    // TO DO: Switch body
    QPushButton* switchButton = new QPushButton("K.REPLACE", this);
    switchButton->setObjectName("primaryButton");
    layout->addWidget(switchButton);
    connect(switchButton, &QPushButton::clicked, [=]() {
        QWidget* newBody = new QLabel("Body replaced !");
        layoutContainer->setBody(newBody);
    });
}