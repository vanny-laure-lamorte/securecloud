#pragma once
#include <optional>
#include <string>

struct MessageModel
{
    int messageId{};
    int senderId{};

    std::optional<int> receiverUserId;
    std::optional<int> receiverGroupId;

    std::string content;
    std::string createdAt;
    std::string updatedAt;
};
