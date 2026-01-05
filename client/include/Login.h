#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

#include "LanguageSelector.h"

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = nullptr);
};

#endif // LOGIN_H
