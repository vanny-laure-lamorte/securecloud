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

private:
    DbConnection &db_;
};
