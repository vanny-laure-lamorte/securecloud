#pragma once
#include "core/ClientService.h"

class ClientService;

class ClientConsoleApp
{
public:
    explicit ClientConsoleApp(ClientService& service);

    void run();

private:
    void printMenu();

    void handleLogin();
    void handleLogout();
    void handleRegister();

    void handleAuthPing();

    void handleAuditAll();
    void handleAuditRefresh();
    void handleAuditPingAuth();
    void handleAuditPingMessaging();

    void handleWsSend();

    void handleMessagingGroups();
    void handleMessagingAllMessages();

private:
    ClientService& service_;
    bool running_ = true;
};
