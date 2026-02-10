#include "NotConnectedLayout.h"
#include <QLabel>
#include <QPushButton>

NotConnectedLayout::NotConnectedLayout(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);

    // Header
    // header = new Header(this);
    // mainLayout->addWidget(header);

    // Body
    body = new Login(this);
    mainLayout->addWidget(body);

    // body = new Home(this);
    // mainLayout->addWidget(body);

    // Footer
    // footer = new Footer(this);
    // mainLayout->addWidget(footer);
}

void NotConnectedLayout::setBody(QWidget *newBody)
{
    if (!newBody) return;
    mainLayout->removeWidget(body);
    body->deleteLater();
    body = newBody;
    mainLayout->insertWidget(1, body);
}
