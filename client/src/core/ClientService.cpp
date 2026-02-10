#include "core/ClientService.h"

ClientService::ClientService(AuthClient& auth,
                             AuditClient& audit,
                             MessagingApiClient& messaging,
                             WsGatewayClient* wsClient,
                             QObject* qtContext)
    : auth_(auth),
      audit_(audit),
      messaging_(messaging),
      wsClient_(wsClient),
      qtContext_(qtContext)
{
}

bool ClientService::login(const std::string& email, const std::string& password)
{
    const bool ok = auth_.login(email, password);
    if (ok)
    {
        wsConnectWithJwtOnQtThread(auth_.getJwt());
    }
    return ok;
}

bool ClientService::logout()
{
    if (!auth_.state_ || !auth_.state_->authenticated)
        return false;
    return auth_.logout(auth_.state_->email);
}

bool ClientService::registerUser(const std::string& email,
                                 const std::string& password,
                                 const std::string& username,
                                 const std::string& firstName,
                                 const std::string& lastName,
                                 const std::string& dob)
{
    return auth_.registerUser(email, password, username, firstName, lastName, dob);
}

void ClientService::pingAuth()
{
    auth_.ping();
}

void ClientService::getServicesStatus()
{
    audit_.getAllServicesStatus();
}

void ClientService::refreshServices()
{
    audit_.refreshServices();
}

void ClientService::pingService(const std::string& serviceName)
{
    audit_.pingService(serviceName);
}

QVector<QMap<int, QString>> ClientService::getGroups()
{
    QVector<QMap<int, QString>> groupsOut;

    const auto groups = messaging_.getGroupsNameForUser(userId());
    for (const auto& group : groups)
    {
        QMap<int, QString> oneGroup;
        for (const auto& [groupId, groupName] : group)
        {
            oneGroup.insert(groupId, QString::fromStdString(groupName));
        }
        groupsOut.append(oneGroup);
    }

    return groupsOut;
}

void ClientService::getAllMessages()
{
    messaging_.getAllMessages();
}

void ClientService::sendPersonal(int targetUserId, const std::string& msg)
{
    if (!wsClient_ || !auth_.state_)
        return;

    wsSendPersonalOnQtThread(auth_.state_->userId, targetUserId, msg);
}

void ClientService::sendGroup(int targetGroupId, const std::string& msg)
{
    if (!wsClient_ || !auth_.state_)
        return;

    wsSendGroupOnQtThread(auth_.state_->userId, targetGroupId, msg);
}

bool ClientService::isAuthenticated() const
{
    return auth_.state_ && auth_.state_->authenticated;
}

int ClientService::userId() const
{
    return auth_.state_ ? auth_.state_->userId : -1;
}

std::string ClientService::email() const
{
    return auth_.state_ ? auth_.state_->email : std::string{};
}


void ClientService::wsConnectWithJwtOnQtThread(const std::string& jwt)
{
    if (!qtContext_ || !wsClient_)
        return;

    QMetaObject::invokeMethod(qtContext_, [this, jwt]()
    {
        wsClient_->connectWithJwt(jwt);
    }, Qt::QueuedConnection);
}

void ClientService::wsSendPersonalOnQtThread(int fromUserId, int targetUserId, const std::string& msg)
{
    if (!qtContext_ || !wsClient_)
        return;

    QMetaObject::invokeMethod(qtContext_, [this, fromUserId, targetUserId, msg]()
    {
        wsClient_->sendPersonal(fromUserId, targetUserId, msg);
    }, Qt::QueuedConnection);
}

void ClientService::wsSendGroupOnQtThread(int fromUserId, int groupId, const std::string& msg)
{
    if (!qtContext_ || !wsClient_)
        return;

    QMetaObject::invokeMethod(qtContext_, [this, fromUserId, groupId, msg]()
    {
        wsClient_->sendGroup(fromUserId, groupId, msg);
    }, Qt::QueuedConnection);
}
