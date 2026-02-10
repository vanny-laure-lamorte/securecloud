#ifndef HOME_H
#define HOME_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QIcon>
#include <QScrollArea>
#include <QLineEdit>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QWidget>

#include "core/ClientService.h"
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
    explicit Home(ClientService* service, QWidget *parent = nullptr);

signals:
    void logoutRequested();
};

#endif // HOME_H
