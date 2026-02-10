#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

#include "LanguageSelector.h"
#include "ChatPage.h"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);

signals:
    void loginRequested(const QString &email, const QString &password);
};

#endif // LOGIN_H
