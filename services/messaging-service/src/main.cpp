#include <drogon/drogon.h>
#include <json/json.h>

#include "common/db/DbConnection.h"
#include "common/Env/EnvLoader.h"
#include "repository/MessageRepository.h"
#include "repository/GroupRepository.h"

int main()
{
    using namespace drogon;

    std::string connStr = env::loadEnv("messaging");
    DbConnection db(connStr, 2);

    MessageRepository messageRepo(db);
    GroupRepository groupRepo(db);

    app().addListener("0.0.0.0", 8082);

    app().registerHandler(
        "/messaging/ping",
        [](const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&cb)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("accessing messaging service");
            cb(resp);
        },
        {Get});

    app().registerHandler(
        "/messaging/groups",
        [&groupRepo](const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&cb)
        {
            auto groups = groupRepo.getAllGroups();

            Json::Value arr(Json::arrayValue);
            for (const auto &g : groups)
            {
                Json::Value j;
                j["group_id"]   = g.groupId;
                j["name"]       = g.name;
                j["created_at"] = g.createdAt;
                j["updated_at"] = g.updatedAt;
                arr.append(j);
            }

            auto resp = HttpResponse::newHttpJsonResponse(arr);
            resp->setStatusCode(k200OK);
            cb(resp);
        },
        {Get});

    app().registerHandler(
        "/messaging/messages",
        [&messageRepo](const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&cb)
        {
            auto msgs = messageRepo.getAllMessages();

            Json::Value arr(Json::arrayValue);
            for (const auto &m : msgs)
            {
                Json::Value j;
                j["message_id"] = m.messageId;
                j["sender_id"]  = m.senderId;

                if (m.receiverUserId.has_value())
                    j["receiver_user_id"] = *m.receiverUserId;
                else
                    j["receiver_user_id"] = Json::nullValue;

                if (m.receiverGroupId.has_value())
                    j["receiver_group_id"] = *m.receiverGroupId;
                else
                    j["receiver_group_id"] = Json::nullValue;

                j["content"]    = m.content;
                j["created_at"] = m.createdAt;
                j["updated_at"] = m.updatedAt;

                arr.append(j);
            }

            auto resp = HttpResponse::newHttpJsonResponse(arr);
            resp->setStatusCode(k200OK);
            cb(resp);
        },
        {Get});

    app().run();
    return 0;
}
