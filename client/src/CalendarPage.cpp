#include "CalendarPage.h"
#include <QHBoxLayout>
#include <QLabel>

CalendarPage::CalendarPage(QWidget *parent)
    : QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("Calendar", this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color:white;font-size:24px;");
        layout->addWidget(label);
}
