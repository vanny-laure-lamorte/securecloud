#include "repository/GroupRepository.h"
#include <drogon/orm/Result.h>
#include <stdexcept>

std::vector<GroupModel> GroupRepository::getAllGroups() const
{
    auto client = db_.client();

    drogon::orm::Result res = client->execSqlSync(
        "SELECT group_id, name, created_at, updated_at FROM groups ORDER BY group_id");

    std::vector<GroupModel> groups;
    for (const auto &row : res)
    {
        GroupModel g;
        g.groupId   = row["group_id"].as<int>();
        g.name      = row["name"].as<std::string>();
        g.createdAt = row["created_at"].as<std::string>();
        g.updatedAt = row["updated_at"].as<std::string>();
        groups.push_back(std::move(g));
    }
    return groups;
}

GroupModel GroupRepository::getGroupById(int groupId) const
{
    auto client = db_.client();

    drogon::orm::Result res = client->execSqlSync(
        "SELECT group_id, name, created_at, updated_at FROM groups WHERE group_id = $1",
        groupId);

    if (res.empty())
        throw std::runtime_error("Group not found");

    GroupModel g;
    g.groupId   = res[0]["group_id"].as<int>();
    g.name      = res[0]["name"].as<std::string>();
    g.createdAt = res[0]["created_at"].as<std::string>();
    g.updatedAt = res[0]["updated_at"].as<std::string>();
    return g;
}