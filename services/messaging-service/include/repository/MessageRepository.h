#pragma once
#include <vector>
#include <optional>
#include "common/db/DbConnection.h"
#include "models/MessageModel.h"

class MessageRepository
{
public:
    explicit MessageRepository(DbConnection &db) : db_(db) {}

    /**
     * @brief Retrieve all messages from the database.
     * @return A vector of MessageModel representing all messages.
     */
    std::vector<MessageModel> getAllMessages() const;

    /**
     * @brief Retrieve personal messages for a specific user.
     * @param userId The ID of the user.
     * @return A vector of MessageModel representing personal messages.
     */
    std::vector<MessageModel> getPersonalMessagesForUser(int userId) const;

    /**
     * @brief Retrieve group messages for a specific group.
     * @param groupId The ID of the group.
     * @return A vector of MessageModel representing group messages.
     */
    std::vector<MessageModel> getGroupMessages(int groupId) const;

    /**
     * @brief Retrieve inbox messages for a specific user (both personal and group messages).
     * @param userId The ID of the user.
     * @return A vector of MessageModel representing inbox messages.
     */
    std::vector<MessageModel> getInboxForUser(int userId) const;

private:
    DbConnection &db_;
};
