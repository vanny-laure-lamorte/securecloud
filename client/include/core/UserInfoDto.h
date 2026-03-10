#ifndef USERINFODTO_H
#define USERINFODTO_H

#include <string>

struct UserInfoDto
{
    std::string username;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string roleName;
    std::string roleDescription;
    std::string createdAt;
    std::string updatedAt;
};

#endif