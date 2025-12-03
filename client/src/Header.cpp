#include "Header.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QPixmap>

Header::Header(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(10);

    // TO DO: replace with the right logo and add App name
    logoLabel = new QLabel(this);
    QPixmap logo(":/assets/logo.png");
    logoLabel->setPixmap(logo.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel* label = new QLabel("Header : Zone fixe", this);
    layout->addWidget(label);
    layout->addWidget(logoLabel);
    setLayout(layout);
}
