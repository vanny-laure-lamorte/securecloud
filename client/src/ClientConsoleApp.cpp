#include "ClientConsoleApp.h"
#include "core/ClientService.h"

#include <iostream>
#include <limits>
#include <string>

ClientConsoleApp::ClientConsoleApp(ClientService& service)
    : service_(service)
{
}

void ClientConsoleApp::printMenu()
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

void ClientConsoleApp::handleLogin()
{
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::string email;
    std::string password;

    std::cout << "Email: ";
    std::getline(std::cin, email);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    if (service_.login(email, password))
        std::cout << "[Client] Login OK.\n";
    else
        std::cout << "[Client] Login FAILED.\n";
}

void ClientConsoleApp::handleLogout()
{
    if (!service_.isAuthenticated())
    {
        std::cout << "[Client] Not logged in.\n";
        return;
    }

    if (service_.logout())
        std::cout << "[Client] Logout OK.\n";
    else
        std::cout << "[Client] Logout FAILED.\n";
}

void ClientConsoleApp::handleRegister()
{
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::string email, password, username, firstName, lastName;

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

    if (service_.registerUser(email, password, username, firstName, lastName))
        std::cout << "[Client] Registration OK.\n";
    else
        std::cout << "[Client] Registration FAILED.\n";
}

void ClientConsoleApp::handleAuthPing()
{
    service_.pingAuth();
}

void ClientConsoleApp::handleAuditAll()
{
    service_.getServicesStatus();
}

void ClientConsoleApp::handleAuditRefresh()
{
    service_.refreshServices();
}

void ClientConsoleApp::handleAuditPingAuth()
{
    service_.pingService("auth");
}

void ClientConsoleApp::handleAuditPingMessaging()
{
    service_.pingService("messaging");
}

void ClientConsoleApp::handleWsSend()
{
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    std::cout << "Send to (u for user, g for group): ";
    char choice;
    std::cin >> choice;

    if (choice != 'u' && choice != 'g')
    {
        std::cout << "[Client] Invalid choice.\n";
        return;
    }

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
        service_.sendPersonal(targetId, msg);
    else
        service_.sendGroup(targetId, msg);
}

void ClientConsoleApp::handleMessagingGroups()
{
    service_.getGroups();
}

void ClientConsoleApp::handleMessagingAllMessages()
{
    service_.getAllMessages();
}

void ClientConsoleApp::run()
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
