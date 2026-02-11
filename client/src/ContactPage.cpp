#include "ContactPage.h"
#include <QHBoxLayout>
#include <QLabel>

ContactPage::ContactPage(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* label = new QLabel("Contact", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color:white;font-size:24px;");
    layout->addWidget(label);
    layout->addStretch();
}
