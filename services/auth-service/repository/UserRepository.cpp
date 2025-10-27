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
