#pragma once

#include <string>

class QObject;
class AuthClient;
class AuditClient;
class MessagingApiClient;
class WsGatewayClient;

class ClientService
{
public:
    ClientService(AuthClient& auth,
                  AuditClient& audit,
                  MessagingApiClient& messaging,
                  WsGatewayClient* wsClient,
                  QObject* qtContext);

    bool login(const std::string& email, const std::string& password);
    bool logout();

    bool registerUser(const std::string& email,
                      const std::string& password,
                      const std::string& username,
                      const std::string& firstName,
                      const std::string& lastName);

    void pingAuth();

    void getServicesStatus();
    void refreshServices();
    void pingService(const std::string& serviceName);

    void getGroups();
    void getAllMessages();

    void sendPersonal(int targetUserId, const std::string& message);
    void sendGroup(int targetGroupId, const std::string& message);

    bool isAuthenticated() const;
    int userId() const;
    std::string email() const;

private:
    void wsConnectWithJwtOnQtThread(const std::string& jwt);
    void wsSendPersonalOnQtThread(int fromUserId,
                                  int targetUserId,
                                  const std::string& msg);
    void wsSendGroupOnQtThread(int fromUserId,
                               int groupId,
                               const std::string& msg);

private:
    AuthClient& auth_;
    AuditClient& audit_;
    MessagingApiClient& messaging_;
    WsGatewayClient* wsClient_ = nullptr;

    QObject* qtContext_ = nullptr;
};
