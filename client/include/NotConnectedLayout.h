#ifndef NOTCONNECTEDLAYOUT_H
#define NOTCONNECTEDLAYOUT_H

#include <QWidget>
#include <QVBoxLayout>

#include "Footer.h"
#include "Header.h"
#include "Login.h"
#include "Home.h"
#include "core/ClientService.h"

class NotConnectedLayout : public QWidget
{
    Q_OBJECT

public:
    explicit NotConnectedLayout(ClientService* service, QWidget *parent = nullptr);

    void setBody(QWidget* newBody);

signals:
    void loginSucceeded();
    void loginFailed(const QString& reason);

private:
    void wireBody(QWidget* bodyWidget);
    ClientService* service_;
    QWidget* header;
    QWidget* body;
    QWidget* footer;
    QVBoxLayout* mainLayout;
};

#endif // NOTCONNECTEDLAYOUT_H
