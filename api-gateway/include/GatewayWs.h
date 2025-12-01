#pragma once
#include <drogon/WebSocketController.h>
#include <drogon/WebSocketClient.h>
#include <memory>

struct WsSession
{
    drogon::WebSocketConnectionPtr frontConn;
    drogon::WebSocketClientPtr     backendClient;
};

class GatewayWs : public drogon::WebSocketController<GatewayWs> {
public:
    void handleNewConnection(const drogon::HttpRequestPtr &req,
                             const drogon::WebSocketConnectionPtr &wsConn) override;

    void handleConnectionClosed(const drogon::WebSocketConnectionPtr &wsConn) override;

    void handleNewMessage(const drogon::WebSocketConnectionPtr &wsConn,
                          std::string &&message,
                          const drogon::WebSocketMessageType &type) override;

    WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws/messaging", drogon::Get);
    WS_PATH_LIST_END

private:
    std::shared_ptr<WsSession> getSession(const drogon::WebSocketConnectionPtr &wsConn);
    void removeSession(const drogon::WebSocketConnectionPtr &wsConn);
    std::unordered_map<drogon::WebSocketConnection *, std::shared_ptr<WsSession>> sessions_;
    std::mutex sessionsMutex_;
};
