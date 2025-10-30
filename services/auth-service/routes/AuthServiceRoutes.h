#pragma once

#include <drogon/drogon.h>
#include "common/db/DbConnection.h"

#include "../repository/UserRepository.h"

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

    UserRepository userRepository_;
private:
};
