#pragma once
#include "common/db/DbConnection.h"
#include "../repository/UserRepository.h"
#include "../service/UserService.h"

#include <drogon/drogon.h>

/**
 * @brief Class to register authentication service routes.
 */
class AuthServiceRoutes
{
public:
    AuthServiceRoutes(DbConnection &db_);

    /**
     * @brief Registers the authentication service routes.
     */
    void registerRoutes();

    private:
    UserService userService_;
};
