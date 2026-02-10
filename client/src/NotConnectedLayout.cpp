#include "NotConnectedLayout.h"
#include <QLabel>
#include <QPushButton>

NotConnectedLayout::NotConnectedLayout(ClientService* service, QWidget *parent)
    : QWidget(parent), service_(service)
{
    mainLayout = new QVBoxLayout(this);

    // Header
    // header = new Header(this);
    // mainLayout->addWidget(header);

    // Body
    body = new Login(this);
    mainLayout->addWidget(body);

    // body = new Home(this);
    // mainLayout->addWidget(body);

    // Footer
    // footer = new Footer(this);
    // mainLayout->addWidget(footer);
    wireBody(body);
}

void NotConnectedLayout::setBody(QWidget *newBody)
{
    if (!newBody) return;
    mainLayout->removeWidget(body);
    body->deleteLater();
    body = newBody;
    mainLayout->insertWidget(1, body);
}

void NotConnectedLayout::wireBody(QWidget* bodyWidget)
{
    if (auto loginPage = qobject_cast<Login*>(bodyWidget))
    {
        connect(loginPage, &Login::loginRequested, this,
                [this](const QString& email, const QString& password)
        {
            if (!service_) return;
            bool ok = service_->login(email.toStdString(), password.toStdString());
            if (ok){
                emit loginSucceeded();
                qDebug() << "Login successful for user:" << email;
                setBody(new Home(this));
            } else {
                emit loginFailed(tr("LOGIN.LOGIN_FAILED"));
                qDebug() << "Login failed for user:" << email;
            }
        });

        connect(loginPage, &Login::registerRequested, this,
                [this](const QString& email, const QString& password, const QString& firstName, const QString& lastName, const QString& dob)
        {
            if (!service_) return;
            std::string username = firstName.toStdString() + "." + lastName.toStdString();
            bool ok = service_->registerUser(email.toStdString(), password.toStdString(), username, firstName.toStdString(), lastName.toStdString(), dob.toStdString());
            if (ok){
                emit registerSucceeded();
                qDebug() << "Register successful for user:" << email;
                setBody(new Home(this));
            } else {
                emit registerFailed(tr("LOGIN.LOGIN_FAILED"));
                qDebug() << "Register failed for user:" << email;
            }
        });
    }
}