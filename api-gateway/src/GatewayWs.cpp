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
                                    const WebSocketConnectionPtr &frontConn)
{
    std::cout << "[GatewayWS] New client WS connection from "
              << req->peerAddr().toIpPort() << "\n";

    auto session = std::make_shared<WsSession>();
    session->frontConn = frontConn;

    session->backendClient = WebSocketClient::newWebSocketClient("127.0.0.1", 8082);

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
            std::cout << "[GatewayWS] Backend WS closed\n";
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
                std::cout << "[GatewayWS] Connected to messaging backend\n";
            }
            else
            {
                std::cout << "[GatewayWS] Failed to connect backend WS: "
                          << (resp ? resp->getStatusCode() : kUnknown) << "\n";
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
    std::cout << "[GatewayWS] Front WS closed\n";
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
        std::cout << "[GatewayWS] No backend client for this session\n";
        frontConn->send("Backend not connected");
        return;
    }

    auto backConn = session->backendClient->getConnection();
    if (backConn && backConn->connected())
    {
        backConn->send(message);
    }
    else
    {
        std::cout << "[GatewayWS] Backend WS not connected yet\n";
        frontConn->send("Backend not connected");
    }
}