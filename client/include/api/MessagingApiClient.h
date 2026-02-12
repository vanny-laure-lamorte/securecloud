#pragma once
#include "core/HttpGatewayClient.h"
#include "core/HttpUtils.h"
#include <json/json.h>
#include <string>
#include <vector>
#include <map>

struct GroupInfo { int id; std::string name; };

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
    std::vector<std::map<int, std::string>> getGroupsNameForUser(int userId);

private:
    HttpGatewayClient &http_;
};
