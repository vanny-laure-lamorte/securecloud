// USERSERVICE.H
#pragma once
#include "../repository/UserRepository.h"
#include "../model/UserProfile.h"
#include "../utils/HashUtils.h"

#include <drogon/orm/DbClient.h>
#include <drogon/drogon.h>
#include <iostream>

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

    /**
     * @brief Login a user with email and password.
     * @param email The email of the user.
     * @param plainPassword The plain text password of the user.
     * @return true if login is successful, false otherwise.
     */
    bool loginUser(const std::string &email,
                   const std::string &plainPassword);

    /**
     * @brief Logout a user by user ID.
     * @param userId The ID of the user.
     * @return true if logout is successful, false otherwise.
     */
    bool logoutUser(const int userId);
    /**
     * @brief Retrieve user information by email.
     * @param email The email of the user.
     * @return A vector of strings containing user information.
     */
    std::vector<std::string> getuserInformations(const std::string &email);
private:
    UserRepository userRepository_;
};