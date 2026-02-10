#include "MessagingWs.h"
#include <json/json.h>

using namespace drogon;



void MessagingWs::init(DbConnection &db)
{
    std::lock_guard<std::mutex> lock(reposMutex_);
    messageRepo_ = std::make_shared<MessageRepository>(db);
    groupRepo_ = std::make_shared<GroupRepository>(db);
}

std::shared_ptr<MessageRepository> MessagingWs::msgRepo()
{
    std::lock_guard<std::mutex> lock(reposMutex_);
    return messageRepo_;
}

std::shared_ptr<GroupRepository> MessagingWs::grpRepo()
{
    std::lock_guard<std::mutex> lock(reposMutex_);
    return groupRepo_;
}

static bool parseJson(const std::string &s, Json::Value &out)
{
    Json::CharReaderBuilder b;
    std::string errs;
    auto r = std::unique_ptr<Json::CharReader>(b.newCharReader());
    return r->parse(s.data(), s.data() + s.size(), &out, &errs);
}

void MessagingWs::handleNewConnection(const HttpRequestPtr &,
                                      const WebSocketConnectionPtr &wsConn)
{
        std::cout << "[Messaging] New WS connection: " << wsConn.get() << "\n";
    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        sessions_[wsConn.get()] = UserSession{wsConn, -1};
    }

    std::cout << "[Messaging] New WS connection\n";
    wsConn->send("Messaging service connected");
}

void MessagingWs::handleConnectionClosed(const WebSocketConnectionPtr &wsConn)
{
    int userId = -1;
    std::cout << "[Messaging] WS connection closed: " << wsConn.get() << "\n";
    {
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        auto it = sessions_.find(wsConn.get());
        if (it != sessions_.end())
        {
            userId = it->second.userId;
            sessions_.erase(it);
        }
    }

    if (userId != -1)
    {
        std::lock_guard<std::mutex> lock(usersMutex_);
        auto it = users_.find(userId);
        if (it != users_.end() && it->second.get() == wsConn.get())
            users_.erase(it);
    }

    std::cout << "[Messaging] WS connection closed\n";
}

void MessagingWs::handleNewMessage(const WebSocketConnectionPtr &wsConn,
                                   std::string &&message,
                                   const WebSocketMessageType &type)
{
    if (type != WebSocketMessageType::Text)
        return;

    Json::Value json;
    if (!parseJson(message, json) || !json.isObject() || !json.isMember("type"))
    {
        wsConn->send(R"({"error":"invalid_json"})");
        return;
    }

    const std::string msgType = json["type"].asString();

    if (msgType == "identify")
    {
        if (!json.isMember("userId"))
        {
            wsConn->send(R"({"error":"missing_userId"})");
            return;
        }

        int userId = -1;
        if (json["userId"].isInt())
            userId = json["userId"].asInt();
        else if (json["userId"].isString())
            userId = std::stoi(json["userId"].asString());

        {
            std::lock_guard<std::mutex> lock(sessionsMutex_);
            sessions_[wsConn.get()].userId = userId;
        }
        {
            std::lock_guard<std::mutex> lock(usersMutex_);
            users_[userId] = wsConn;
        }

        wsConn->send(R"({"type":"identify.ok"})");
        return;
    }

    auto getSenderId = [&]() -> int
    {
        if (json.isMember("userId"))
        {
            if (json["userId"].isInt())
                return json["userId"].asInt();
            if (json["userId"].isString())
                return std::stoi(json["userId"].asString());
        }
        std::lock_guard<std::mutex> lock(sessionsMutex_);
        auto it = sessions_.find(wsConn.get());
        return (it != sessions_.end()) ? it->second.userId : -1;
    };

    const int senderId = getSenderId();
    if (senderId == -1)
    {
        wsConn->send(R"({"error":"not_identified"})");
        return;
    }

    if (msgType == "msg.personal")
    {
        if (!json.isMember("toUserId") || !json.isMember("content"))
        {
            wsConn->send(R"({"error":"missing_toUserId_or_content"})");
            return;
        }

        int toUserId = json["toUserId"].isInt()
                           ? json["toUserId"].asInt()
                           : std::stoi(json["toUserId"].asString());

        const std::string content = json["content"].asString();

        // WebSocketConnectionPtr target;
        // {
        //     std::lock_guard<std::mutex> lock(usersMutex_);
        //     auto it = users_.find(toUserId);
        //     if (it != users_.end())
        //         target = it->second;
        // }

        if (!toUserId)
        {
            wsConn->send(R"({"type":"msg.personal.ack","status":"offline"})");
            return;
        }

        Json::Value out;
        out["type"] = "msg.personal.recv";
        out["fromUserId"] = senderId;
        out["content"] = content;

        // Json::StreamWriterBuilder w;
        // w["indentation"] = "";
        // target->send(Json::writeString(w, out));

        wsConn->send(R"({"type":"msg.personal.ack","status":"sent"})");
        try {
            auto repo = msgRepo();
            if (!repo) {
                std::cerr << "[Messaging] messageRepo not initialized\n";
            } else {
                repo->insertPersonalMessage(toUserId, senderId, content);
            }
        } catch (const std::exception &e) {
            std::cerr << "Failed to insert personal message: " << e.what() << std::endl;
        }
        return;
    }

    if (msgType == "msg.group")
    {
        if (!json.isMember("groupId") || !json.isMember("content"))
        {
            wsConn->send(R"({"error":"missing_groupId_or_content"})");
            return;
        }

        const int groupId = json["groupId"].isInt()
                                ? json["groupId"].asInt()
                                : std::stoi(json["groupId"].asString());

        const std::string content = json["content"].asString();

        Json::Value out;
        out["type"] = "msg.group.recv";
        out["groupId"] = groupId;
        out["fromUserId"] = senderId;
        out["content"] = content;

        Json::StreamWriterBuilder w;
        w["indentation"] = "";
        const std::string payload = Json::writeString(w, out);

        std::lock_guard<std::mutex> lock(usersMutex_);
        for (auto &kv : users_)
        {
            auto &c = kv.second;
            if (c && c->connected())
                c->send(payload);
        }

        wsConn->send(R"({"type":"msg.group.ack","status":"sent"})");
        try {
            auto repo = msgRepo();
            if (!repo) {
                std::cerr << "[Messaging] messageRepo not initialized\n";
            } else {
                repo->insertGroupMessage(groupId, senderId, content);
            }
        } catch (const std::exception &e) {
            std::cerr << "Failed to insert group message: " << e.what() << std::endl;
        }
        return;
    }

    wsConn->send(R"({"error":"unknown_type"})");
}
