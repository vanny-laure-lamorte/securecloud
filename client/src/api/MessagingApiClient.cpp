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
