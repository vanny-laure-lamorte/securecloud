#pragma once
#include <drogon/drogon.h>
#include <string>
#include <utility>
#include <chrono>

class ClientApp {
public:
    // Constructor: takes an HTTP client instance
    explicit ClientApp(const drogon::HttpClientPtr& client);

    // Main loop: shows menu, reads user input, sends requests
    void run();

private:
    bool running = true;                  // Controls the main loop
    drogon::HttpClientPtr client_;        // HTTP client to send requests

    // Print the interactive menu to the terminal
    static void printMenu();

    // Send a request and wait for the response (with timeout)
    // Returns (HTTP status code, response body)
    std::pair<int, std::string>
    sendAndWait(const drogon::HttpRequestPtr& req,
                std::chrono::seconds timeout = std::chrono::seconds(5));

    // Send a GET request to a given path and print the result
    void ping(const std::string& path);

    // Send a POST request to /audit/events with a sample JSON payload
    void postAuditEvent();

    // Send a GET request to /audit/services and print parsed status
    void getServicesStatus();

    // Send a POST request to /audit/services to refresh services status
    void postServicesStatus();
};
