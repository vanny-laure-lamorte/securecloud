#pragma once
#include <drogon/drogon.h>
#include <string>
#include <utility>
#include <chrono>

class ClientApp {
public:
    explicit ClientApp(const drogon::HttpClientPtr& client);
    void run();

private:
    bool running = true;
    drogon::HttpClientPtr client_;

    static void printMenu();
    std::pair<int, std::string>
    sendAndWait(const drogon::HttpRequestPtr& req,
                std::chrono::seconds timeout = std::chrono::seconds(5));

    void ping(const std::string& path);
    void postAuditEvent();
    void getServicesStatus();
    void postServicesStatus();
};
