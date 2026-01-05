// USERSERVICE.H

#pragma once
#include <drogon/orm/DbClient.h>
#include <drogon/drogon.h>
#include <iostream>

#include "../repository/UserRepository.h"
#include "../model/UserProfile.h"
#include "../utils/HashUtils.h"

using namespace drogon;
class UserService
{
public:
    UserService(DbConnection &dbConnection)
        : userRepository_(dbConnection) {}

    /**
     * @brief Register a new user with email, password, username, first name, and last name.
     * @param email The email of the user.
     * @param plainPassword The plain text password of the user.
     * @param username The username of the user.
     * @param firstName The first name of the user.
     * @param lastName The last name of the user.
     */
    void registerUser(const std::string &email,
                      const std::string &plainPassword,
                      const std::string &username,
                      const std::string &firstName,
                      const std::string &lastName);

    bool loginUser(const std::string &email,
                   const std::string &plainPassword);

    bool logoutUser(const int userId);

    std::vector<std::string> getuserInformations(const std::string &email);
private:
    UserRepository userRepository_;
};