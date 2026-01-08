#include "repository/MessageRepository.h"
#include <drogon/orm/Result.h>

static MessageModel rowToMessage(const drogon::orm::Row &row)
{
    MessageModel m;
    m.messageId = row["message_id"].as<int>();
    m.senderId  = row["sender_id"].as<int>();

    if (!row["receiver_user_id"].isNull())
        m.receiverUserId = row["receiver_user_id"].as<int>();
    if (!row["receiver_group_id"].isNull())
        m.receiverGroupId = row["receiver_group_id"].as<int>();

    m.content   = row["content"].as<std::string>();
    m.createdAt = row["created_at"].as<std::string>();
    m.updatedAt = row["updated_at"].as<std::string>();
    return m;
}

std::vector<MessageModel> MessageRepository::getAllMessages() const
{
    auto client = db_.client();
    auto res = client->execSqlSync(
        "SELECT message_id, sender_id, receiver_user_id, receiver_group_id, content, created_at, updated_at "
        "FROM messages "
        "ORDER BY created_at ASC"
    );

    std::vector<MessageModel> out;
    out.reserve(res.size());
    for (const auto &row : res)
        out.push_back(rowToMessage(row));
    return out;
}

std::vector<MessageModel> MessageRepository::getPersonalMessagesForUser(int userId) const
{
    auto client = db_.client();
    auto res = client->execSqlSync(
        "SELECT message_id, sender_id, receiver_user_id, receiver_group_id, content, created_at, updated_at "
        "FROM messages "
        "WHERE receiver_group_id IS NULL "
        "AND (sender_id = $1 OR receiver_user_id = $1) "
        "ORDER BY created_at ASC",
        userId
    );

    std::vector<MessageModel> out;
    out.reserve(res.size());
    for (const auto &row : res)
        out.push_back(rowToMessage(row));
    return out;
}

std::vector<MessageModel> MessageRepository::getGroupMessages(int groupId) const
{
    auto client = db_.client();
    auto res = client->execSqlSync(
        "SELECT message_id, sender_id, receiver_user_id, receiver_group_id, content, created_at, updated_at "
        "FROM messages "
        "WHERE receiver_group_id = $1 "
        "ORDER BY created_at ASC",
        groupId
    );

    std::vector<MessageModel> out;
    out.reserve(res.size());
    for (const auto &row : res)
        out.push_back(rowToMessage(row));
    return out;
}

std::vector<MessageModel> MessageRepository::getInboxForUser(int userId) const
{
    return getPersonalMessagesForUser(userId);
}
