#pragma once
#include <vector>
#include <string>

#include "common/db/DbConnection.h"
#include "models/GroupModel.h"

class GroupRepository
{
public:
    explicit GroupRepository(DbConnection &db) : db_(db) {}

    /**
     * @brief Retrieve all groups.
     * @return A vector of GroupModel representing all groups.
     */
    std::vector<GroupModel> getAllGroups() const;

    /**
     * @brief Retrieve a group by its id.
     * @param groupId The ID of the group.
     * @return The GroupModel representing the group.
     */
    GroupModel getGroupById(int groupId) const;

    /**
     * @brief Retrieve groups a user belongs to (FUTURE).
     * TODO: add group_members table
     */
    std::vector<GroupModel> getGroupsForUser(int userId) const;

    /**
     * @brief Create a new group with the given name.
     * @param name The name of the new group.
     */
    void insertGroup(const std::string &name);

private:
    DbConnection &db_;
};
