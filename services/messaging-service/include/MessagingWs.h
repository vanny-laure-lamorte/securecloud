#pragma once
#include <drogon/WebSocketController.h>
#include <unordered_set>
#include <mutex>

/**
 * Session data for a connected user.
 */
struct UserSession {
    drogon::WebSocketConnectionPtr conn;
    std::string userId;
};

class MessagingWs : public drogon::WebSocketController<MessagingWs> {
public:

    /**
     * Handles a new WebSocket connection from a client.
     * @param req The HTTP request that initiated the WebSocket connection.
     * @param wsConn The WebSocket connection to the client.
     */
    void handleNewConnection(const drogon::HttpRequestPtr &,
                             const drogon::WebSocketConnectionPtr &wsConn) override;

    /**
     * Handles the closing of a WebSocket connection from a client.
     * @param wsConn The WebSocket connection that was closed.
     */
    void handleConnectionClosed(const drogon::WebSocketConnectionPtr &wsConn) override;

    /**
     * Handles a new message received from a client WebSocket connection.
     * @param wsConn The WebSocket connection from which the message was received.
     * @param message The message received from the client.
     * @param type The type of the WebSocket message (text, binary, etc.).
     */
    void handleNewMessage(const drogon::WebSocketConnectionPtr &wsConn,
                          std::string &&message,
                          const drogon::WebSocketMessageType &type) override;

    /**
     * WebSocket path mappings for this controller.
     */
    WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws/messaging", drogon::Get);
    WS_PATH_LIST_END

private:
    std::unordered_map<drogon::WebSocketConnection*, UserSession> sessions_;
    std::mutex sessionsMutex_;
    std::unordered_set<drogon::WebSocketConnectionPtr> conns_;
    std::mutex connsMutex_;
};
