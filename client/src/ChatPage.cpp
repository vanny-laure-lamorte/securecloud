#include "ChatPage.h"
#include <QHBoxLayout>
#include <QLabel>

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* label = new QLabel("Chat", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color:white;font-size:24px;");
    layout->addWidget(label);
    layout->addStretch();
}
