#include "api/MessagingApiClient.h"
#include <iostream>

using namespace drogon;

static void printResponseRaw(const std::string &body, size_t maxLen = 4000)
{
    std::cout << "Response Body (" << body.size() << " bytes):\n";
    if (body.size() <= maxLen)
    {
        std::cout << body << "\n";
    }
    else
    {
        std::cout << body.substr(0, maxLen) << "\n... (truncated)\n";
    }
}

bool MessagingApiClient::getGroups()
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/messaging/groups");

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Messaging", "groups", code, body);
    printResponseRaw(body);
    return HttpUtils::isSuccess(code);
}

bool MessagingApiClient::getInbox(int userId)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/messaging/messages/inbox?userId=" + std::to_string(userId));

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Messaging", "inbox", code, body);

    printResponseRaw(body);
    return HttpUtils::isSuccess(code);
}

bool MessagingApiClient::getGroupMessages(int groupId)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/messaging/messages/group?groupId=" + std::to_string(groupId));

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Messaging", "groupMessages", code, body);

    printResponseRaw(body);
    return HttpUtils::isSuccess(code);
}

bool MessagingApiClient::getAllMessages()
{
    auto req = HttpRequest::newHttpRequest();
    std::cout << "Creating request for all messages\n";
    req->setMethod(Get);
    req->setPath("/messaging/messages");

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Messaging", "allMessages", code, body);

    printResponseRaw(body);
    return HttpUtils::isSuccess(code);
}

std::vector<std::pair<int, std::string>> MessagingApiClient::getGroupsNameForUser(int userId)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/messaging/users/" + std::to_string(userId) + "/groups");

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Messaging", "groupsNameForUser", code, body);

    printResponseRaw(body);
    if (!HttpUtils::isSuccess(code))
        return {};

    std::vector<std::pair<int, std::string>> groups;
    try
    {
        Json::Value jsonResponse;
        Json::CharReaderBuilder readerBuilder;
        std::string errs;
        std::istringstream s(body);
        if (Json::parseFromStream(readerBuilder, s, &jsonResponse, &errs))
        {
            for (const auto &group : jsonResponse)
            {
                int groupId = group["group_id"].asInt();
                std::string groupName = group["name"].asString();
                groups.emplace_back(std::make_pair(groupId, groupName));
            }
        }
        else
        {
            std::cerr << "Failed to parse JSON: " << errs << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception while parsing JSON: " << e.what() << "\n";
    }

    return groups;
}

std::vector<std::pair<int, std::string>> MessagingApiClient::getMessagesForGroupOrUser(int userId, std::string type, int id)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/messaging/users/" + std::to_string(userId) + "/type/" + type + "/messages/" + std::to_string(id));

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Messaging", "messagesForGroupOrUser", code, body);

    printResponseRaw(body);
    if (!HttpUtils::isSuccess(code))
        return {};

    std::vector<std::pair<int, std::string>> messages;
    try
    {
        Json::Value jsonResponse;
        Json::CharReaderBuilder readerBuilder;
        std::string errs;
        std::istringstream s(body);

        if (Json::parseFromStream(readerBuilder, s, &jsonResponse, &errs))
        {
            for (const auto &message : jsonResponse)
            {
                int senderId = message["sender_id"].asInt();
                std::string content = message["content"].asString();
                messages.emplace_back(std::make_pair(senderId, content));
            }
        } else
        {
            std::cerr << "Failed to parse JSON: " << errs << "\n";
        }
    } catch (const std::exception &e)
    {
        std::cerr << "Exception while parsing JSON: " << e.what() << "\n";
    }
    return messages;
}