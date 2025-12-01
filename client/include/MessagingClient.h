#pragma once
#include <QCoreApplication>
#include <QWebSocket>
#include <QTimer>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

class MessagingClient : public QObject
{
    Q_OBJECT
public:
    explicit MessagingClient(const QString& userId, QObject* parent = nullptr)
        : QObject(parent),
          userId_(userId)
    {
        connect(&ws_, &QWebSocket::connected, this, &MessagingClient::onConnected);
        connect(&ws_, &QWebSocket::disconnected, this, &MessagingClient::onDisconnected);
        connect(&ws_, &QWebSocket::textMessageReceived,
                this, &MessagingClient::onMessageReceived);
        connect(&ws_, &QWebSocket::errorOccurred,
                this, &MessagingClient::onError);
    }

    void maybeConnect(bool auth)
    {
        if (!auth) {
            qDebug() << "Not authenticated, no WS connection.";
            return;
        }
        QUrl url(QStringLiteral("ws://127.0.0.1:8080/ws/messaging"));
        qDebug() << "Connecting to" << url << "as user" << userId_;
        ws_.open(url);
    }

    void sendTestMessage(const QString& msg)
    {
        if (ws_.state() == QAbstractSocket::ConnectedState) {
            ws_.sendTextMessage(msg);
        } else {
            qDebug() << "WS not connected, can't send.";
        }
    }

private slots:
    void onConnected()
    {
        qDebug() << "[Qt] WS connected to gateway";

        QJsonObject identify;
        identify["type"] = "identify";
        identify["userId"] = userId_;

        QJsonDocument doc(identify);
        ws_.sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));

        // Message de test
        ws_.sendTextMessage("Hello from Qt client via gateway!");
    }

    void onDisconnected()
    {
        qDebug() << "[Qt] WS disconnected";
    }

    void onMessageReceived(const QString& msg)
    {
        qDebug() << "[Qt] WS message:" << msg;
    }

    void onError(QAbstractSocket::SocketError)
    {
        qDebug() << "[Qt] WS error:" << ws_.errorString();
    }

private:
    QWebSocket ws_;
    QString userId_;
};
