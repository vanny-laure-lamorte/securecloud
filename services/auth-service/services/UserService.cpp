#include "UserService.h"

void UserService::registerUser(const std::string &email,
                               const std::string &password,
                               const std::string &username,
                               const std::string &firstName,
                               const std::string &lastName)
{
    if (userRepository_.getUserExists(email)){
        std::cout << "User already exists with email: " << email << std::endl;
        throw std::runtime_error("USER_ALREADY_EXISTS");
    }
        std::string salt = HashUtils::generateSalt(10);
        std::string hashedPassword = HashUtils::hashPassword(password, salt);

    try
    {
        userRepository_.insertUser(username, firstName, lastName);

        int userId = userRepository_.getUserIdByUsername(username);
        if (!userId)
        {
            throw std::runtime_error("Cannot retrieve user_id after insertion");
        }
        userRepository_.insertConnexion(email, hashedPassword, salt, userId);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error registering user: " << e.what() << std::endl;
        throw;
    }
}

bool UserService::loginUser(const std::string &email,
                            const std::string &plainPassword)
{
    try
    {
        std::string salt = userRepository_.getSaltByEmail(email);
        std::string hashed = HashUtils::hashPassword(plainPassword, salt);
        return userRepository_.checkCredentials(email, hashed);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[UserService] Login failed: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> UserService::getuserInformations (const std::string &email)
{
    std::vector<std::string> userInfo;
    try
    {
        std::string username = userRepository_.getUsernameByEmail(email);
        userRepository_.setLastSeen(userRepository_.getUserIdByUsername(username));
        userInfo.push_back(username);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[UserService] Get user information failed: " << e.what() << std::endl;
    }
    return userInfo;
}

bool UserService::logoutUser(const int userId){
    try
    {
        userRepository_.setLastSeen(userId);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[UserService] Logout failed: " << e.what() << std::endl;
        return false;
    }
}