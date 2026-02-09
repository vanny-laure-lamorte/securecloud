#include "GroupPage.h"
#include <QHBoxLayout>
#include <QLabel>

GroupPage::GroupPage(QWidget *parent)
    : QWidget(parent)
{
      QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("Group", this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color:white;font-size:24px;");
        layout->addWidget(label);
}
