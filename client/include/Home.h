#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include "ActivityPage.h"
#include "CalendarPage.h"
#include "ChatPage.h"
#include "ContactPage.h"
#include "EmergencyPage.h"
#include "FilterPage.h"
#include "GroupPage.h"
#include "SettingPage.h"

class Home : public QWidget
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = nullptr);
};

#endif // HOME_H
