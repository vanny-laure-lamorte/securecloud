#pragma once
#include "common/db/DbConnection.h"
#include <drogon/drogon.h>
using namespace drogon;

class UserRepository
{
public:
    explicit UserRepository(DbConnection &dbConnection)
        : db_(dbConnection) {};

    /*
    * Add a user
    */
    void addUser(const std::string &email,
                 const std::string &hashedPassword,
                 int userId);

    /**
     * Get username by email
     * @param email User email
     * @return username
     */
    std::string getUsernameByEmail(const std::string &email);

private:

    DbConnection &db_;
};
