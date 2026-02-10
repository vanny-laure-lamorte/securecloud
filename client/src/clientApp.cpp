#include "ClientApp.h"
#include "api/AuthClient.h"
#include "api/AuditClient.h"
#include "api/MessagingApiClient.h"
#include "core/WsGatewayClient.h"

#include <iostream>
#include <limits>
#include <string>

ClientApp::ClientApp(AuthClient &auth,
                     AuditClient &audit,
                     MessagingApiClient &messaging,
                     WsGatewayClient *wsClient)
    : auth_(auth),
      audit_(audit),
      messaging_(messaging),
      wsClient_(wsClient)
{
}

void ClientApp::printMenu()
{
    std::cout << "Menu:\n"
                 "  1) Login via API Gateway (/auth/login)\n"
                 "  2) Register via API Gateway (/auth/register)\n"
                 "  3) Ping auth via API Gateway (/auth/ping)\n"
                 "  4) GET services status (DB) via API Gateway (/audit/services)\n"
                 "  5) POST services refresh via API Gateway (/audit/services)\n"
                 "  6) POST -> Ping Auth service via Audit-service (/audit/service_ping: auth)\n"
                 "  7) POST -> Ping Messaging service via Audit-service (/audit/service_ping: messaging)\n"
                 "  8) Send WebSocket message via gateway\n"
                 "  9) Logout\n"
                 "  10) GET messaging groups (/messaging/groups)\n"
                 "  11) GET ALL messages (/messaging/messages)\n"
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
        if (wsClient_)
            wsClient_->connectWithJwt(auth_.getJwt());
    }
    else
    {
        std::cout << "[Client] Login FAILED.\n";
    }
}

//TODO : Fix crash on logout (Probably due to wsClient_ using invalid jwt after logout)
void ClientApp::handleLogout(){
    if(auth_.state_->authenticated){
        if(auth_.logout(auth_.state_->email)){
            std::cout << "[Client] Logout OK.\n";
            // running_ = false;
        } else {
            std::cout << "[Client] Logout FAILED.\n";
        }
    }
}

void ClientApp::handleRegister()
{
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::string email;
    std::string password;
    std::string username;
    std::string firstName;
    std::string lastName;

    std::cout << "Email: ";
    std::getline(std::cin, email);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "First Name: ";
    std::getline(std::cin, firstName);

    std::cout << "Last Name: ";
    std::getline(std::cin, lastName);

    if (auth_.registerUser(email, password, username, firstName, lastName))
        std::cout << "[Client] Registration OK.\n";
    else
        std::cout << "[Client] Registration FAILED.\n";
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

    std::cout << "Send to (u for user, g for group): ";
    char choice;
    std::cin >> choice;
    if (choice != 'u' && choice != 'g')
    {
        std::cout << "[Client] Invalid choice.\n";
        return;
    }
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::cout << "Enter user ID or group ID: ";
    int targetId;
    std::cin >> targetId;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::cout << "Enter WS message: ";
    std::string msg;
    std::getline(std::cin, msg);

    if (msg.empty())
    {
        std::cout << "[Client] Empty message, nothing sent.\n";
        return;
    }

    if (choice == 'u')
        wsClient_->sendPersonal(auth_.state_->userId, targetId, msg);
    else
        wsClient_->sendGroup(auth_.state_->userId, targetId, msg);
}

void ClientApp::handleMessagingGroups()
{
    messaging_.getGroups();
}

void ClientApp::handleMessagingAllMessages()
{
    messaging_.getAllMessages();
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
            handleLogout();
            std::cout << "[Client] Bye.\n";
            return;
        case 1:
            handleLogin();
            break;
        case 2:
            handleRegister();
            break;
        case 3:
            handleAuthPing();
            break;
        case 4:
            handleAuditAll();
            break;
        case 5:
            handleAuditRefresh();
            break;
        case 6:
            handleAuditPingAuth();
            break;
        case 7:
            handleAuditPingMessaging();
            break;
        case 8:
            handleWsSend();
            break;
        case 9:
            handleLogout();
            break;
        case 10:
            handleMessagingGroups();
            break;
        case 11:
            handleMessagingAllMessages();
            break;
        default:
            std::cout << "[Client] Invalid choice.\n";
            break;
        }
    }
}
