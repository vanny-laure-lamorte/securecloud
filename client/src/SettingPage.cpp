#include "SettingPage.h"
#include <QHBoxLayout>
#include <QLabel>

SettingPage::SettingPage(QWidget *parent)
    : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("Settings", this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color:white;font-size:24px;");
        layout->addWidget(label);
}
