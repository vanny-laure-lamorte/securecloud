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

void WsGatewayClient::sendJson(const QString &jsonText)
{
    if (!impl_) return;

    QMetaObject::invokeMethod(impl_, [impl = impl_, jsonText]() {
        impl->sendMessage(jsonText);
    }, Qt::QueuedConnection);
}

void WsGatewayClient::sendPersonal(int userId, int toUserId, const std::string &content)
{
    QJsonObject o;
    o["type"] = "msg.personal";
    o["userId"] = userId;
    o["toUserId"] = toUserId;
    o["content"] = QString::fromStdString(content);

    sendJson(QString::fromUtf8(QJsonDocument(o).toJson(QJsonDocument::Compact)));
}

void WsGatewayClient::sendGroup(int userId, int groupId, const std::string &content)
{
    QJsonObject o;
    o["type"] = "msg.group";
    o["userId"] = userId;
    o["groupId"] = groupId;
    o["content"] = QString::fromStdString(content);

    sendJson(QString::fromUtf8(QJsonDocument(o).toJson(QJsonDocument::Compact)));
}
