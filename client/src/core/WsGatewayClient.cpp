#include "core/WsGatewayClient.h"
#include "core/MessagingClient.h"

#include <QString>
#include <QDebug>
#include <QMetaObject>

WsGatewayClient::WsGatewayClient(MessagingClient *impl)
    : impl_(impl)
{
}

bool WsGatewayClient::connectWithJwt(const std::string &jwt)
{
    if (!impl_)
    {
        qWarning("WsGatewayClient: MessagingClient is null");
        return false;
    }

    QString qjwt = QString::fromStdString(jwt);
    QMetaObject::invokeMethod(
        impl_,
        [impl = impl_, qjwt]() {
            impl->maybeConnect(qjwt);
        },
        Qt::QueuedConnection
    );
    return true;
}

void WsGatewayClient::send(const std::string &msg)
{
    if (!impl_)
    {
        qWarning("WsGatewayClient: MessagingClient is null");
        return;
    }

    QString qmsg = QString::fromStdString(msg);
    QMetaObject::invokeMethod(
        impl_,
        [impl = impl_, qmsg]() {
            impl->sendTestMessage(qmsg);
        },
        Qt::QueuedConnection
    );
}
