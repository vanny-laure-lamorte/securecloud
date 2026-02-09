#include "ActivityPage.h"
#include <QHBoxLayout>
#include <QLabel>

ActivityPage::ActivityPage(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* label = new QLabel("Activity", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color:white;font-size:24px;");
    layout->addWidget(label);
    layout->addStretch();
}
