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

std::vector<std::map<int, std::string>> MessagingApiClient::getGroupsNameForUser(int userId)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/messaging/users/" + std::to_string(userId) + "/groups");
    // TODO : add userId as query parameter when the endpoint is implemented in the service
    // req->setPath("/messaging/groups");

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Messaging", "groupsNameForUser", code, body);

    printResponseRaw(body);
    if (!HttpUtils::isSuccess(code))
        return {};

    std::vector<std::map<int, std::string>> groups;
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
                groups.push_back({{groupId, groupName}});
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
