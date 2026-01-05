#include "UserRepository.h"

void UserRepository::insertUser(
    const std::string &username,
    const std::string &first_name,
    const std::string &last_name)
{
    auto client = db_.client();
    try
    {
        drogon::orm::Result res = client->execSqlSync(
            "INSERT INTO users(username, first_name, last_name) "
            "VALUES ($1, $2, $3) "
            "RETURNING user_id",
            username,
            first_name,
            last_name);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error adding user: " << e.what() << std::endl;
    }
}

void UserRepository::insertConnexion(
    const std::string &email,
    const std::string &hashedPassword,
    const std::string &salt,
    int userId)
{
    auto client = db_.client();
    try
    {
        client->execSqlSync(
            "INSERT INTO connexion(email, hashed_password, salt, user_id) "
            "VALUES ($1, $2, $3, $4)",
            email,
            hashedPassword,
            salt,
            userId);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error adding connexion: " << e.what() << std::endl;
        throw;
    }
}

std::string UserRepository::getUsernameByEmail(const std::string &email)
{
    auto client = db_.client();
    drogon::orm::Result res = client->execSqlSync(
        "SELECT u.username "
        "FROM connexion c "
        "JOIN users u ON u.user_id = c.user_id "
        "WHERE c.email = $1",
        email);

    if (res.empty())
    {
        throw std::runtime_error("User not found for email: " + email);
    }

    return res[0]["username"].as<std::string>();
}

int UserRepository::getUserIdByUsername(
    const std::string &username)
{
    auto client = db_.client();
    drogon::orm::Result res = client->execSqlSync(
        "SELECT user_id FROM users WHERE username = $1", username);

    if (res.empty())
    {
        throw std::runtime_error("UserID not found for username: " + username);
    }

    return res[0]["user_id"].as<int>();
}

bool UserRepository::checkCredentials(
    const std::string &email,
    const std::string &hashedPassword)
{
    auto client = db_.client();
    drogon::orm::Result res = client->execSqlSync(
        "SELECT COUNT(*) AS count "
        "FROM connexion "
        "WHERE email = $1 AND hashed_password = $2",
        email,
        hashedPassword);

    if (res.empty())
    {
        return false;
    }

    int count = res[0]["count"].as<int>();
    return count > 0;
}

std::string UserRepository::getSaltByEmail(const std::string &email)
{
    auto client = db_.client();
    drogon::orm::Result res = client->execSqlSync(
        "SELECT salt FROM connexion WHERE email = $1",
        email);

    if (res.empty())
    {
        throw std::runtime_error("Salt not found for email: " + email);
    }

    return res[0]["salt"].as<std::string>();
}

UserProfile UserRepository::userInfo(const std::string &email)
{
    auto client = db_.client();
    drogon::orm::Result res = client->execSqlSync(
        "SELECT u.username, u.first_name, u.last_name, c.email "
        "FROM users u "
        "JOIN connexion c ON u.user_id = c.user_id "
        "WHERE c.email = $1",
        email);

    if (res.empty())
    {
        throw std::runtime_error("User not found for email: " + email);
    }

    UserProfile profile;
    profile.username = res[0]["username"].as<std::string>();
    profile.firstName = res[0]["first_name"].as<std::string>();
    profile.lastName = res[0]["last_name"].as<std::string>();
    profile.email = res[0]["email"].as<std::string>();
    return profile;
}

void UserRepository::setLastSeen(int userId){
    auto client = db_.client();
    try
    {
        client->execSqlSync(
            "UPDATE users SET last_seen = NOW() WHERE user_id = $1",
            userId);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error updating last seen: " << e.what() << std::endl;
        throw;
    }
}

bool UserRepository::getUserExists(std::string const &email){
    auto client = db_.client();
    drogon::orm::Result res = client->execSqlSync(
        "SELECT COUNT(*) AS count FROM connexion WHERE email = $1",
        email);

    if (res.empty())
    {
        return false;
    }

    int count = res[0]["count"].as<int>();
    return count > 0;
}