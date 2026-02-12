#pragma once
#include <drogon/WebSocketController.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include "repository/MessageRepository.h"
#include "repository/GroupRepository.h"
#include "common/db/DbConnection.h"

/**
 * Session data for a connected user.
 */
struct UserSession {
    drogon::WebSocketConnectionPtr conn;
    int userId = -1;
};

class MessagingWs : public drogon::WebSocketController<MessagingWs> {
public:

    ~MessagingWs() override
    {
        std::cout << "[Messaging] MessagingWs destroyed\n";
    }

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

    static void init(DbConnection &db);

    static int getUserId(const drogon::WebSocketConnectionPtr &wsConn);

protected:
    static std::shared_ptr<MessageRepository> msgRepo();

    static std::shared_ptr<GroupRepository> grpRepo();

private:
    inline static std::shared_ptr<MessageRepository> messageRepo_{};
    inline static std::shared_ptr<GroupRepository> groupRepo_{};
    inline static std::mutex reposMutex_{};

    inline static std::unordered_map<drogon::WebSocketConnection*, UserSession> sessions_{};
    inline static std::mutex sessionsMutex_{};

    inline static std::unordered_map<int, drogon::WebSocketConnectionPtr> users_{};
    inline static std::mutex usersMutex_{};
};
