#pragma once
#include <atomic>
#include <memory>
#include <drogon/WebSocketController.h>
#include <drogon/WebSocketClient.h>

/**
 * Session data for a WebSocket connection
 * between a frontend client and backend messaging service.
 */
struct WsSession {
    drogon::WebSocketConnectionPtr frontConn;
    drogon::WebSocketClientPtr backendClient;
    bool backendReady = false;
    std::vector<std::string> pendingMessages;
    std::mutex pendingMutex;
    std::string token;
    std::atomic_bool closing{false};
    std::atomic_bool connecting{false};};

class GatewayWs : public drogon::WebSocketController<GatewayWs> {
public:
    /**
     * Handles a new WebSocket connection from a client.
     * Establishes a backend WebSocket connection to the messaging service
     * and sets up message forwarding between the client and backend.
     * @param req The HTTP request that initiated the WebSocket connection (handshake).
     * @param frontConn The WebSocket connection to the client.
     */
    void handleNewConnection(const drogon::HttpRequestPtr &req,
                             const drogon::WebSocketConnectionPtr &wsConn) override;

    /**
     * Handles the closing of a WebSocket connection from a client.
     * Cleans up the associated backend connection and session data.
     * @param wsConn The WebSocket connection that was closed.
     */
    void handleConnectionClosed(const drogon::WebSocketConnectionPtr &wsConn) override;

    /**
     * Handles a new message received from a client WebSocket connection.
     * Forwards the message to the backend messaging service if connected.
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

    /**
     * Retrieves the session associated with a given WebSocket connection.
     * @param wsConn The WebSocket connection.
     */
    std::shared_ptr<WsSession> getSession(const drogon::WebSocketConnectionPtr &wsConn);

    /**
     * Removes the session associated with a given WebSocket connection.
     * @param wsConn The WebSocket connection to remove.
     */
    void removeSession(const drogon::WebSocketConnectionPtr &wsConn);

    std::unordered_map<drogon::WebSocketConnection *, std::shared_ptr<WsSession>> sessions_;
    std::mutex sessionsMutex_;
};
