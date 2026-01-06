// ui/ClientApp.h
#pragma once
#include <functional>
#include <string>

class AuthClient;
class AuditClient;
class WsGatewayClient;

class ClientApp
{
public:
    ClientApp(AuthClient &auth,
              AuditClient &audit,
              WsGatewayClient *wsClient = nullptr);

    /**
     * Runs the main application loop.
     */
    void run();

private:
    /**
     * Prints the main menu options.
     */
    void printMenu();

    /**
     * Handles user login.
     */
    void handleLogin();

    /**
     * Handles user registration.
     */
    void handleRegister();

    /**
     * Handles authentication service ping.
     */
    void handleAuthPing();

    /**
     * Handle services status retrieval.
     */
    void handleAuditAll();

    /**
     * Handle audit service refresh.
     */

    void handleAuditRefresh();

    /**
     * Handles pinging the auth service via the audit service.
     */
    void handleAuditPingAuth();

    /**
     * Handles pinging the messaging service via the audit service.
     */
    void handleAuditPingMessaging();

    /**
     * Handles sending a message via WebSocket.
     */
    void handleWsSend();

    /**
     * Handles user logout.
     */
    void handleLogout();

    AuthClient &auth_;
    AuditClient &audit_;
    WsGatewayClient *wsClient_;
    bool running_ = true;
};
