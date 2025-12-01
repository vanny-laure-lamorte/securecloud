#pragma once
#include <drogon/WebSocketController.h>
#include <unordered_set>
#include <mutex>

struct UserSession {
    drogon::WebSocketConnectionPtr conn;
    std::string userId;
};

class MessagingWs : public drogon::WebSocketController<MessagingWs> {
public:
    void handleNewConnection(const drogon::HttpRequestPtr &,
                             const drogon::WebSocketConnectionPtr &wsConn) override;

    void handleConnectionClosed(const drogon::WebSocketConnectionPtr &wsConn) override;

    void handleNewMessage(const drogon::WebSocketConnectionPtr &wsConn,
                          std::string &&message,
                          const drogon::WebSocketMessageType &type) override;

    WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws/messaging", drogon::Get);
    WS_PATH_LIST_END

private:
    std::unordered_map<drogon::WebSocketConnection*, UserSession> sessions_;
    std::mutex sessionsMutex_;
    std::unordered_set<drogon::WebSocketConnectionPtr> conns_;
    std::mutex connsMutex_;
};
