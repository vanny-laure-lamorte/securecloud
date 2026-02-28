// USERPROFILE

#pragma once
#include <iostream>
#include <ctime>

struct UserProfile {
    std::string username;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::time_t createdAt;
    std::time_t updatedAt;
    std::string roleName;
    std::string roleDescription;
    std::string jwt;
};

struct UserSession {
    std::string username;
    std::string email;
    std::string jwt;
    std::time_t lastSeen;
    std::time_t createdAt;
};