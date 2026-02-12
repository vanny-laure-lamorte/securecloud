#pragma once

#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

class MessagingClient;

/**
 * WebSocket Gateway Client.
 * Wraps a MessagingClient to provide WebSocket connectivity
 * to the API Gateway.
 * Uses JWT for authentication.
 */
class WsGatewayClient
{
public:
    WsGatewayClient(MessagingClient*);

    /**
     * Connects to the WebSocket gateway using the provided JWT.
     * @param jwt The JWT token for authentication.
     * @param userId The user ID for identification (optional, can be included in the JWT).
     */
    bool connectWithJwt(const std::string &jwt, int userId);

    /**
     * Sends a personnal message via the WebSocket connection.
     * @param toUserId The recipient user ID.
     * @param content The message to send.
     */
    void sendPersonal(int userId, int toUserId, const std::string &content);

    /**
     * Sends a group message via the WebSocket connection.
     * @param groupId The recipient group ID.
     * @param content The message to send.
     */
    void sendGroup(int userId, int groupId, const std::string &content);

private:
    /**
     * Sends a raw JSON message via the WebSocket connection.
     * @param jsonText The JSON text to send.
     */
    void sendJson(const QString &jsonText);
    MessagingClient *impl_;
};
