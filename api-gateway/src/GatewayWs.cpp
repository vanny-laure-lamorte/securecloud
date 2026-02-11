#include "GatewayWs.h"

#include <drogon/drogon.h>
#include <iostream>

using namespace drogon;


static void sendJson(const WebSocketConnectionPtr &c, const std::string &json)
{
    if (c && c->connected())
        c->send(json);
}

static void connectBackend(const std::shared_ptr<WsSession> &session);


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
    auto session = std::make_shared<WsSession>();
    session->frontConn = frontConn;
    session->backendReady = false;
    session->closing.store(false);
    session->connecting.store(false);

    if (req)
        session->token = req->getParameter("token");

    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        sessions_[frontConn.get()] = session;
    }

    std::cout << "[GatewayWs] new front WS connection\n";

    session->backendClient = WebSocketClient::newWebSocketClient("ws://messaging-service-api:8082");

    session->backendClient->setMessageHandler(
        [session](const std::string &message,
                  const WebSocketClientPtr &,
                  const WebSocketMessageType &type)
        {
            if (type != WebSocketMessageType::Text)
                return;

            if (session->frontConn && session->frontConn->connected())
                session->frontConn->send(message);
        });

    session->backendClient->setConnectionClosedHandler(
        [session](const WebSocketClientPtr &)
        {
            std::cout << "[GatewayWs] backend WS closed\n";
            session->backendReady = false;

            if (session->closing.load())
                return;

            sendJson(session->frontConn, R"({"type":"ws.backend.down"})");

            connectBackend(session);
        });

    connectBackend(session);
}

void GatewayWs::handleConnectionClosed(const WebSocketConnectionPtr &frontConn)
{
    std::cout << "[GatewayWs] front WS closed\n";

    auto session = getSession(frontConn);
    if (!session)
    {
        removeSession(frontConn);
        return;
    }

    session->closing.store(true);

    if (session->backendClient)
    {
        auto backConn = session->backendClient->getConnection();
        if (backConn && backConn->connected())
            backConn->shutdown();

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
        sendJson(frontConn, R"({"error":"no_session"})");
        return;
    }

    if (session->closing.load())
        return;

    auto backConn = session->backendClient->getConnection();
    if (backConn && backConn->connected() && session->backendReady)
    {
        backConn->send(message);
    }
    else
    {
        {
            std::lock_guard<std::mutex> lock(session->pendingMutex);
            session->pendingMessages.push_back(std::move(message));
        }
        sendJson(frontConn, R"({"type":"ws.buffered"})");

        connectBackend(session);
    }
}

static void connectBackend(const std::shared_ptr<WsSession> &session)
{
    if (!session || session->closing.load())
        return;

    bool expected = false;
    if (!session->connecting.compare_exchange_strong(expected, true))
        return;

    auto reqBackend = HttpRequest::newHttpRequest();
    reqBackend->setMethod(Get);

    if (!session->token.empty())
        reqBackend->setPath("/ws/messaging?token=" + session->token);
    else
        reqBackend->setPath("/ws/messaging");

    session->backendClient->connectToServer(
        reqBackend,
        [session](ReqResult r,
                  const HttpResponsePtr &resp,
                  const WebSocketClientPtr &wsPtr)
        {
            session->connecting.store(false);

            std::cout << "[GatewayWs] backend connect result=" << (int)r;
            if (resp)
                std::cout << " httpStatus=" << resp->statusCode();
            std::cout << "\n";

            if (session->closing.load())
                return;

            if (r != ReqResult::Ok)
            {
                session->backendReady = false;
                sendJson(session->frontConn, R"({"type":"ws.backend.connect_failed"})");
                return;
            }

            session->backendReady = true;
            sendJson(session->frontConn, R"({"type":"ws.backend.up"})");

            auto backConn = wsPtr->getConnection();
            if (!backConn || !backConn->connected())
                return;

            std::vector<std::string> pending;
            {
                std::lock_guard<std::mutex> lock(session->pendingMutex);
                pending.swap(session->pendingMessages);
            }
            for (const auto &msg : pending)
                backConn->send(msg);
        });
}
