// core/HttpGatewayClient.h
#pragma once
#include "core/ClientState.h"
#include <drogon/drogon.h>
#include <chrono>
#include <string>
#include <utility>

class HttpGatewayClient
{
public:
    /**
     * @brief Construct a new Http Gateway Client object
     * @param client The underlying drogon HttpClient
     * @param state Shared client state (authentication, JWT, etc.)
     */
    HttpGatewayClient(const drogon::HttpClientPtr &client,
                      std::shared_ptr<ClientState> state)
        : client_(client), state_(std::move(state)) {}

    /**
     * @brief Send an HTTP request via the gateway client.
     * If a JWT token is set in the client state, it is added as a Bearer
     * token in the Authorization header.
     * @param req The HTTP request to send.
     * @param timeout Timeout duration for the request (default 5 seconds).
     */
    std::pair<int, std::string> send(const drogon::HttpRequestPtr &req,
                                     std::chrono::seconds timeout = std::chrono::seconds(5));

private:
    drogon::HttpClientPtr client_;
    std::shared_ptr<ClientState> state_;
};

