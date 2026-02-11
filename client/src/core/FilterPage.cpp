#include "FilterPage.h"
#include <QHBoxLayout>
#include <QLabel>

FilterPage::FilterPage(QWidget *parent)
    : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("Filter", this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color:white;font-size:24px;");
        layout->addWidget(label);
}
