#pragma once
#include <drogon/drogon.h>
#include <string>
#include <utility>
#include <chrono>

class ClientApp
{
public:

    using WsSendCallback = std::function<void(const std::string&)>;

    // Constructor: takes an HTTP client instance
    explicit ClientApp(const drogon::HttpClientPtr &client,
                       WsSendCallback wsSendCallback = nullptr);

    // Main loop: shows menu, reads user input, sends requests
    void run();

    std::string selectFakeUser();

private:
    bool running = true;           // Controls the main loop
    drogon::HttpClientPtr client_; // HTTP client to send requests
    WsSendCallback wsSendCallback_; // Callback to send messages via WebSocket

    /**
     * @brief Print the interactive menu to the console
     */
    static void printMenu();

    /**
     * @brief Send an HTTP request and wait for the response or timeout
     * @param req The HTTP request to send
     * @param timeout Maximum wait time for the response
     * @return A pair of HTTP status code and response body as string
     */
    std::pair<int, std::string> sendAndWait(
        const drogon::HttpRequestPtr &req,
        std::chrono::seconds timeout = std::chrono::seconds(5));

    /**
     * @brief Send a GET request to the specified path and print the response
     * @param path The request path (e.g., "/auth/ping")
     */
    void ping(const std::string &path);

    /**
     * @brief Send a POST request to /audit/events with a sample audit event and print the response
     */
    void postAuditEvent();

    /**
     * @brief Send a GET request to /audit/services to retrieve and print services status
     */
    void getAllServicesStatus();

    void getOneServiceStatus(const std::string &serviceName);

    /**
     * @brief Send a POST request to /audit/services to refresh services status
     * @note This function triggers a status refresh on the server side.
     */
    void postServicesStatus();

    /**
     * @brief Send a POST request to /audit/service_ping to ping a specific service
     * @param serviceName The name of the service to ping (e.g., "auth")
     */
    void auditServicePing(const std::string &serviceName);
};
