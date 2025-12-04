#pragma once
#include <string>

/**
 * Client state shared across API clients.
 * Holds authentication status, email, JWT, etc.
 */
struct ClientState
{
    bool authenticated = false;

    std::string userNamme;
    std::string email;
    std::string jwt;
};
