#ifndef NOTCONNECTEDLAYOUT_H
#define NOTCONNECTEDLAYOUT_H

#include <QWidget>
#include <QVBoxLayout>

#include "Footer.h"
#include "Header.h"
#include "Login.h"
#include "MainPage.h"

class NotConnectedLayout : public QWidget
{
    Q_OBJECT

public:
    explicit NotConnectedLayout(QWidget *parent = nullptr);

    void setBody(QWidget* newBody);

private:

    QWidget* header;
    QWidget* body;
    QWidget* footer;
    QVBoxLayout* mainLayout;
};

#endif // NOTCONNECTEDLAYOUT_H
