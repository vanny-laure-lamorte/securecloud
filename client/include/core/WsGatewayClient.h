#pragma once

#include <string>
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
    ~WsGatewayClient();

    /**
     * Connects to the WebSocket gateway using the provided JWT.
     * @param jwt The JWT token for authentication.
     */
    bool connectWithJwt(const std::string &jwt);

    /**
     * Sends a message via the WebSocket connection.
     * @param msg The message to send.
     */
    void send(const std::string &msg);

private:
    MessagingClient *impl_;
};
