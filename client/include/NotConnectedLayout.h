#ifndef NOTCONNECTEDLAYOUT_H
#define NOTCONNECTEDLAYOUT_H

#include <QWidget>
#include <QVBoxLayout>

#include "core/ClientService.h"
#include "Footer.h"
#include "Header.h"
#include "Home.h"
#include "Login.h"

class NotConnectedLayout : public QWidget
{
    Q_OBJECT

public:
    explicit NotConnectedLayout(ClientService* service, QWidget *parent = nullptr);

    /**
     * Sets the body widget of the layout, replacing the existing one.
     * @param newBody The new QWidget to set as the body.
     */
    void setBody(QWidget* newBody);

signals:
    /**
     * signal emitted when a login attempt is successful.
     */
    void loginSucceeded();

    /**
     * signal emitted when a login attempt fails, with a reason for the failure.
     * @param reason A QString describing the reason for the login failure.
     */
    void loginFailed(const QString& reason);

    /**
     * signal emitted when a registration attempt is successful.
     */
    void registerSucceeded();

    /**
     * signal emitted when a registration attempt fails, with a reason for the failure.
     * @param reason A QString describing the reason for the registration failure.
     */
    void registerFailed(const QString& reason);

private:
    /**
     * Wires the signals from the body widget (e.g., Login) to the appropriate slots in this layout.
     * @param bodyWidget The QWidget that is currently set as the body, whose signals need
     * to be connected to this layout's slots.
     */
    void wireBody(QWidget* bodyWidget);

    ClientService* service_;
    QWidget* header;
    QWidget* body;
    QWidget* footer;
    QVBoxLayout* mainLayout;
};

#endif // NOTCONNECTEDLAYOUT_H
