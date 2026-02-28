#include "UserService.h"

void UserService::registerUser(const std::string &email,
                               const std::string &password,
                               const std::string &username,
                               const std::string &firstName,
                               const std::string &lastName)
{
    if (userRepository_.getUserExists(email)) {
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

std::vector<std::map<std::string, std::string>> UserService::getuserInformations(int &userId) {
    std::vector<std::map<std::string, std::string>> userInfo;
    try
    {
        UserProfile profile = userRepository_.getUserAllInfo(userId);
        userInfo.push_back({{"username", profile.username}});
        userInfo.push_back({{"firstName", profile.firstName}});
        userInfo.push_back({{"lastName", profile.lastName}});
        userInfo.push_back({{"email", profile.email}});
        userInfo.push_back({{"createdAt", std::to_string(profile.createdAt)}});
        userInfo.push_back({{"updatedAt", std::to_string(profile.updatedAt)}});
        userInfo.push_back({{"roleName", profile.roleName}});
        userInfo.push_back({{"roleDescription", profile.roleDescription}});
    }
    catch (const std::exception &e)
    {
        std::cerr << "[UserService] Get user information failed: " << e.what() << std::endl;
    }
    return userInfo;
}

bool UserService::logoutUser(const int userId) {
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