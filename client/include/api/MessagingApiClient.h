#pragma once
#include "core/HttpGatewayClient.h"
#include "core/HttpUtils.h"
#include <json/json.h>
#include <string>
#include <vector>
#include <map>

struct GroupInfo { int id; std::string name; };
struct MessageInfo { std::string content; int senderId; };

class MessagingApiClient
{
public:
    explicit MessagingApiClient(HttpGatewayClient &http) : http_(http) {}

    /**
     * @brief Retrieve all messaging groups.
     * @return true if the request was successful, false otherwise.
     */
    bool getGroups();

    /**
     * @brief Retrieve inbox messages for a specific user.
     * @param userId The ID of the user.
     * @return true if the request was successful, false otherwise.
     */
    bool getInbox(int userId);

    /**
     * @brief Retrieve messages for a specific group.
     * @param groupId The ID of the group.
     * @return true if the request was successful, false otherwise.
     */
    bool getGroupMessages(int groupId);

    /**
     * @brief Retrieve all messages.
     * @return true if the request was successful, false otherwise.
     */
    bool getAllMessages();

    /**
     * @brief get the names of groups for a specific user.
     * @param userId The ID of the user.
     * @return A vector of maps, where each map contains the group ID and group name for the groups the user is part of.
     */
    std::vector<std::pair<int, std::string>> getGroupsNameForUser(int userId);

    /**
     * @brief Retrieve messages for a specific group or personal conversation.
     * @param userId The ID of the user for whom to retrieve messages.
     * @param type The type of conversation ("group" or "personal").
     * @param id The ID of the group or user to retrieve messages for.
     * @return A vector of maps, where each map represents a message with its details (e.g.content, sender ID).
     */
    std::vector<std::pair<int, std::string>> getMessagesForGroupOrUser(int userId, std::string type, int id);

    /**
     * @brief Retrieve contact IDs for a specific user.
     * @param userId The ID of the user for whom to retrieve contact IDs.
     * @return A list of contact IDs associated with the user.
     */
    std::list<int> getContactIdsForUser(int userId);

private:
    HttpGatewayClient &http_;
};
