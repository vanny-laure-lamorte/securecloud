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
    std::vector<MessageModel> getPersonalMessagesForUser(int userId, int targetId) const;

    /**
     * @brief Retrieve group messages for a specific group.
     * @param groupId The ID of the group.
     * @return A vector of MessageModel representing group messages.
     */
    std::vector<MessageModel> getGroupMessages(int groupId) const;

    /**
     * @brief Insert a new personal message into the database.
     * @param senderId The ID of the sender.
     * @param receiverUserId The ID of the recipient user.
     * @param content The content of the message.
     */
    void insertPersonalMessage(int senderId, int receiverUserId, const std::string &content);

    /**
     * @brief Insert a new group message into the database.
     * @param senderId The ID of the sender.
     * @param receiverGroupId The ID of the recipient group.
     * @param content The content of the message.
     */
    void insertGroupMessage(int senderId, int receiverGroupId, const std::string &content);

    /**
     * @brief Retrieve the contact information for a user by user ID.
     * @param userId The ID of the user.
     * @return A list of integers representing the contact information (e.g., user IDs of contacts).
     */
    std::list<int> getContactsForUser(int userId) const;

private:
    DbConnection &db_;
};
