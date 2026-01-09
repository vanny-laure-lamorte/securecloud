#include "EmergencyPage.h"
#include <QHBoxLayout>
#include <QLabel>

EmergencyPage::EmergencyPage(QWidget *parent)
    : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("Emergency", this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color:white;font-size:24px;");
        layout->addWidget(label);
}
