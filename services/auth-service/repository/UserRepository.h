#pragma once
#include "common/db/DbConnection.h"
#include "../model/UserProfile.h"

#include <ctime>
#include <drogon/drogon.h>
#include <drogon/orm/Result.h>

using namespace drogon;

class UserRepository
{
public:
    explicit UserRepository(DbConnection &dbConnection)
        : db_(dbConnection) {};

    /**
     * @brief Insert a new user into the database.
     * @param username The username of the user.
     * @param firstName The first name of the user.
     * @param lastName The last name of the user.
     */
    void insertUser(const std::string &username,
                    const std::string &firstName,
                    const std::string &lastName);

    /**
     * @brief Insert a new connexion into the database.
     * @param email The email of the user.
     * @param hashedPassword The hashed password of the user.
     * @param userId The ID of the user.
     */
    void insertConnexion(const std::string &email,
                         const std::string &hashedPassword,
                         const std::string &salt,
                         int userId);

    /**
     * @brief Get the user ID by username.
     * @param username The username of the user.
     * @return The user ID if found, std::nullopt otherwise.
     */
    int getUserIdByUsername(const std::string &username);

    /**
     * @brief Get the username by email.
     * @param email The email of the user.
     * @return The username if found, std::nullopt otherwise.
     */
    std::string getUsernameByEmail(const std::string &email);

    /**
     * @brief Check if the provided credentials are valid.
     * @param email The email of the user.
     * @param hashedPassword The hashed password to check.
     * @return true if the credentials are valid, false otherwise.
     */
    bool checkCredentials(const std::string &email,
                          const std::string &hashedPassword);

    /**
     * @brief Get the salt associated with the given email.
     * @param email The email of the user.
     * @return The salt string.
     */
    std::string getSaltByEmail(const std::string &email);

    /**
     * @brief Retrieve user information by email.
     * @param email The email of the user.
     * @return UserProfile struct containing user information.
     */
    UserProfile userInfo(const std::string &email);

    /**
     * @brief Get the last seen timestamp for the user by email.
     * @param email The email of the user.
     * @return time_t representing the last seen timestamp.
     */
    time_t getLastSeen(const std::string &email);

    /**
     * @brief Get the account creation timestamp for the user by email.
     * @param email The email of the user.
     * @return time_t representing the account creation timestamp.
     */
    time_t getCreatedAt(const std::string &email);

    /**
     * @brief Update the last seen timestamp for the user.
     * @param userId The ID of the user.
     */
    void setLastSeen(const int userId);

    /**
     * @brief Check if a user exists with the given email.
     * @param email The email of the user.
     * @return true if the user exists, false otherwise.
     */
    bool getUserExists(const std::string &email);

    /**
     * @brief Get the user ID by email.
     * @param email The email of the user.
     * @return The user ID if found, std::nullopt otherwise.
     */
    int getUserIdByEmail(const std::string &email);

private:
    DbConnection &db_;
};
