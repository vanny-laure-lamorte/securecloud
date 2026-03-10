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

UserInfoDto ClientService::getUserInfos()
{
    return auth_.getUserInfos(userId());
}

QVector<QPair<int, QString>> ClientService::getGroups()
{
    QVector<QPair<int, QString>> groupsOut;
    qDebug() << "getGroups userId =" << userId();
    const auto groups = messaging_.getGroupsNameForUser(userId());
    for (const auto& [groupId, groupName] : groups)
    {
        groupsOut.append(QPair<int, QString>(groupId, QString::fromStdString(groupName)));
    }

    return groupsOut;
}

QVector<QPair<int, QString>> ClientService::getContacts()
{
    QVector<QPair<int, QString>> contactsOut;
    qDebug() << "getContacts userId =" << userId();
    std::list<int> contactIds = messaging_.getContactIdsForUser(userId());
    QSet<int> seen;

    for (int contactId : contactIds)
    {
        if (seen.contains(contactId))
            continue;

        seen.insert(contactId);

        UserDto user = auth_.getUserById(contactId);

        if (user.id != -1 && !user.username.empty())
        {
            contactsOut.append(QPair<int, QString>(
                user.id,
                QString::fromStdString(user.username)
            ));
        }
    }

    return contactsOut;
}

QVector<ChatMessage> ClientService::getMessages(int id, const std::string& type)
{
    QVector<ChatMessage> messagesOut;
    const auto rawMessages = messaging_.getMessagesForGroupOrUser(userId(), type, id);

    QHash<int, QString> senderNamesCache;

    for (const auto& rawMessage : rawMessages)
    {
        int senderId = rawMessage.senderId;
        QString senderName;

        if (!senderNamesCache.contains(senderId))
        {
            UserDto user = auth_.getUserById(senderId);
            senderNamesCache.insert(senderId, QString::fromStdString(user.username));
        }
        senderName = senderNamesCache.value(senderId, "Unknown");

        messagesOut.append(ChatMessage{
            senderId,
            senderName,
            QString::fromStdString(rawMessage.content)
        });
    }

    return messagesOut;
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
        wsClient_->connectWithJwt(jwt, userId());
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
