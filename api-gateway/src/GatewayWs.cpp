#include "GatewayWs.h"

#include <iostream>

using namespace drogon;

std::shared_ptr<WsSession> GatewayWs::getSession(const WebSocketConnectionPtr &wsConn)
{
    std::lock_guard<std::mutex> lock(sessionsMutex_);
    auto it = sessions_.find(wsConn.get());
    if (it == sessions_.end())
        return nullptr;
    return it->second;
}

void GatewayWs::removeSession(const WebSocketConnectionPtr &wsConn)
{
    std::lock_guard<std::mutex> lock(sessionsMutex_);
    sessions_.erase(wsConn.get());
}

void GatewayWs::handleNewConnection(const HttpRequestPtr &req,
                                    const WebSocketConnectionPtr &frontConn){
    auto session = std::make_shared<WsSession>();
    session->frontConn = frontConn;

    session->backendClient = WebSocketClient::newWebSocketClient("ws://messaging-service-api:8082/ws/messaging");

    session->backendClient->setMessageHandler(
        [session](const std::string &message,
                  const WebSocketClientPtr &,
                  const WebSocketMessageType &type)
        {
            if (type != WebSocketMessageType::Text)
                return;

            if (session->frontConn && session->frontConn->connected())
            {
                session->frontConn->send(message);
            }
        });

    session->backendClient->setConnectionClosedHandler(
        [session](const WebSocketClientPtr &)
        {
            if (session->frontConn && session->frontConn->connected())
            {
                session->frontConn->shutdown();
            }
        });

    auto reqBackend = HttpRequest::newHttpRequest();
    reqBackend->setMethod(Get);
    reqBackend->setPath("/ws/messaging");

    session->backendClient->connectToServer(
        reqBackend,
        [session](ReqResult r,
                  const HttpResponsePtr &resp,
                  const WebSocketClientPtr &wsPtr)
        {
            if (r == ReqResult::Ok)
            {

                session->backendReady = true;

                auto backConn = wsPtr->getConnection();
                if (backConn && backConn->connected())
                {
                    for (const auto &msg : session->pendingMessages)
                    {
                        backConn->send(msg);
                    }
                }
                session->pendingMessages.clear();
            }
            else
            {
                if (session->frontConn && session->frontConn->connected())
                {
                    session->frontConn->shutdown();
                }
                wsPtr->stop();
            }
        });

    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        sessions_[frontConn.get()] = session;
    }
}

void GatewayWs::handleConnectionClosed(const WebSocketConnectionPtr &frontConn)
{
    auto session = getSession(frontConn);
    if (session && session->backendClient)
    {
        auto backConn = session->backendClient->getConnection();
        if (backConn && backConn->connected())
        {
            backConn->shutdown();
        }
        session->backendClient->stop();
    }

    removeSession(frontConn);
}

void GatewayWs::handleNewMessage(const WebSocketConnectionPtr &frontConn,
                                 std::string &&message,
                                 const WebSocketMessageType &type)
{
    if (type != WebSocketMessageType::Text)
        return;

    auto session = getSession(frontConn);
    if (!session || !session->backendClient)
    {
        frontConn->send("Messaging service not connected");
        return;
    }

    auto backConn = session->backendClient->getConnection();
    if (backConn && backConn->connected() && session->backendReady)
    {
        backConn->send(message);
    }
    else
    {
        session->pendingMessages.push_back(message);
    }
}
