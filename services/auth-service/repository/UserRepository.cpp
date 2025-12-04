#include "UserRepository.h"

void UserRepository::addUser(
    const std::string &email,
    const std::string &hashedPassword,
    int userId)
{
    try
    {
        std::string sql = "INSERT INTO connexion(email, password, user_id) VALUES ($1, $2, $3)";
        db_.client()->execSqlSync(
            sql,
            email, hashedPassword, userId);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error adding user: " << e.what() << std::endl;
        throw;
    }
}

std::string UserRepository::getUsernameByEmail(const std::string &email)
{
    try
    {
        std::string sql = "SELECT u.username FROM connexion c INNER JOIN users u ON u.userId = c.userId WHERE c.email = $1;";

            auto result = db_.client()->execSqlSync(sql, email);

        if (result.size() == 0)
        {
            throw std::runtime_error("User not found");
        }

        return result[0]["username"].as<std::string>();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error retrieving username: " << e.what() << std::endl;
        throw;
    }
}