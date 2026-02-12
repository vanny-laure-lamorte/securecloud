#pragma once
#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>
#include <QWebSocket>

class MessagingClient : public QObject
{
    Q_OBJECT
public:
    explicit MessagingClient(const QString &userId, QObject *parent = nullptr)
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

    /**
     * @brief Attempts to connect to the messaging WebSocket server using the provided JWT.
     * @param jwt The JSON Web Token for authentication.
     * @return true if the connection attempt was initiated, false if the JWT is empty.
     */
    bool maybeConnect(const QString &jwt, int userId)
    {
        QString userIdToString = QString::number(userId);
        if(userIdToString != userId_)
            userId_ = userIdToString;

        if (jwt.isEmpty())
        {
            qDebug() << "Empty JWT, no WS connection.";
            return false;
        }

        QUrl url(QStringLiteral("ws://127.0.0.1:8080/ws/messaging"));
        QUrlQuery query;
        query.addQueryItem("token", jwt);
        url.setQuery(query);
        qDebug() << "Connecting to" << url << "as user" << userId_;
        ws_.open(url);
        return true;
    }

    /**
     * @brief Sends a test message over the WebSocket connection.
     * @param msg The message to send.
     */
    void sendMessage(const QString &msg)
    {
        if (ws_.state() == QAbstractSocket::ConnectedState)
        {
            ws_.sendTextMessage(msg);
        }
        else
        {
            qDebug() << "WS not connected, can't send.";
        }
    }

    /**
     * @brief Sets the user ID for identification upon connection.
     * @param userId The user ID to set.
     */
    void setUserId(const QString &userId) { userId_ = userId; }

private slots:
    /**
     * @brief Slot called when the WebSocket connection is established.
     */
    void onConnected()
    {
        qDebug() << "[Qt] WS connected to gateway";

        QJsonObject identify;
        identify["type"] = "identify";
        identify["userId"] = userId_;

        QJsonDocument doc(identify);
        ws_.sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    }

    /**
     * @brief Slot called when the WebSocket connection is closed.
     */
    void onDisconnected()
    {
        qDebug() << "[Qt] WS disconnected";
    }

    /**
     * @brief Slot called when a text message is received from the WebSocket server.
     * @param msg The received message.
     */
    void onMessageReceived(const QString &msg)
    {
        qDebug() << "[Qt] WS message:" << msg;
    }

    /**
     * @brief Slot called when a WebSocket error occurs.
     * @param error The type of socket error.
     */
    void onError(QAbstractSocket::SocketError)
    {
        qDebug() << "[Qt] WS error:" << ws_.errorString();
    }

private:
    QWebSocket ws_;
    QString userId_;
};
