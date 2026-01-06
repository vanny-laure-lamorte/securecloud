#include "MessagingWs.h"
using namespace drogon;

void MessagingWs::handleNewConnection(const HttpRequestPtr &,
                                      const WebSocketConnectionPtr &wsConn)
{
    {
        std::lock_guard<std::mutex> lock(connsMutex_);
        conns_.insert(wsConn);
    }
    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        sessions_[wsConn.get()] = UserSession{wsConn, ""};
    }

    std::cout << "[Messaging] New WS connection\n";
    wsConn->send("Messaging service connected to gateway");
}

void MessagingWs::handleConnectionClosed(const WebSocketConnectionPtr &wsConn)
{
    {
        std::lock_guard<std::mutex> lock(connsMutex_);
        conns_.erase(wsConn);
    }
    std::cout << "[Messaging] WS connection closed\n";
}

void MessagingWs::handleNewMessage(const WebSocketConnectionPtr &wsConn,
                                   std::string &&message,
                                   const WebSocketMessageType &type)
{
    if (type != WebSocketMessageType::Text) {
        return;
    }

    std::cout << "[Messaging] Received from one client: " << message << "\n";

    std::lock_guard<std::mutex> lock(connsMutex_);
    for (auto &c : conns_) {
        if (c->connected()) {
            c->send("Echo/Broadcast: " + message);
        }
    }
}
