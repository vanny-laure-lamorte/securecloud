#include "ClientApp.h"
#include "api/AuthClient.h"
#include "api/AuditClient.h"
#include "core/WsGatewayClient.h"

#include <iostream>
#include <limits>
#include <string>

ClientApp::ClientApp(AuthClient &auth,
                     AuditClient &audit,
                     WsGatewayClient *wsClient)
    : auth_(auth),
      audit_(audit),
      wsClient_(wsClient)
{
}

void ClientApp::printMenu()
{
    std::cout << "Menu:\n"
                 "  1) Login via API Gateway (/auth/login)\n"
                 "  2) Ping auth via API Gateway (/auth/ping)\n"
                 "  3) GET services status (DB) via API Gateway (/audit/services)\n"
                 "  4) POST services refresh via API Gateway (/audit/services)\n"
                 "  5) POST -> Ping Auth service via Audit-service (/audit/service_ping: auth)\n"
                 "  6) POST -> Ping Messaging service via Audit-service (/audit/service_ping: messaging)\n"
                 "  7) Send WebSocket message via gateway\n"
                 "  0) Quit\n> ";
}

void ClientApp::handleLogin()
{
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::string email;
    std::string password;

    std::cout << "Email: ";
    std::getline(std::cin, email);

    std::cout << "Password: ";
    std::getline(std::cin, password);
    if (auth_.login(email, password))
    {
        std::cout << "[Client] Login OK.\n";
        wsClient_->connectWithJwt(auth_.getJwt());
    }
    else
    {
        std::cout << "[Client] Login FAILED.\n";
    }
}

void ClientApp::handleAuthPing()
{
    auth_.ping();
}

void ClientApp::handleAuditAll()
{
    audit_.getAllServicesStatus();
}

void ClientApp::handleAuditRefresh()
{
    audit_.refreshServices();
}

void ClientApp::handleAuditPingAuth()
{
    audit_.pingService("auth");
}

void ClientApp::handleAuditPingMessaging()
{
    audit_.pingService("messaging");
}

void ClientApp::handleWsSend()
{
    if (!wsClient_)
    {
        std::cout << "[Client] WebSocket not available.\n";
        return;
    }

    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::cout << "Enter WS message: ";
    std::string msg;
    std::getline(std::cin, msg);

    if (msg.empty())
    {
        std::cout << "[Client] Empty message, nothing sent.\n";
        return;
    }

    wsClient_->send(msg);
}

void ClientApp::run()
{
    while (running_)
    {
        printMenu();
        int choice = -1;
        if (!(std::cin >> choice))
        {
            std::cout << "[Client] Bye.\n";
            break;
        }

        switch (choice)
        {
        case 0:
            std::cout << "[Client] Bye.\n";
            return;
        case 1:
            handleLogin();
            break;
        case 2:
            handleAuthPing();
            break;
        case 3:
            handleAuditAll();
            break;
        case 4:
            handleAuditRefresh();
            break;
        case 5:
            handleAuditPingAuth();
            break;
        case 6:
            handleAuditPingMessaging();
            break;
        case 7:
            handleWsSend();
            break;
        default:
            std::cout << "[Client] Invalid choice.\n";
            break;
        }
    }
}
