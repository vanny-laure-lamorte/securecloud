#pragma once
#include <string>
#include <json/json.h>
#include "core/HttpGatewayClient.h"
#include "core/ClientState.h"

class AuthClient
{
public:
    /**
     * Auth Client constructor.
     * @param http Reference to the HTTP gateway client for making requests.
     * @param state Shared client state (authentication, JWT, etc.)
     */
    AuthClient(HttpGatewayClient &http,
               const std::shared_ptr<ClientState> &state)
        : http_(http), state_(state) {}

    /**
     * Logs in a user with the given email and password.
     * On success, updates the client state with authentication info.
     * @param email User's email.
     * @param password User's password.
     */
    bool login(const std::string &email, const std::string &password);

    /**
     * Logs out the user with the given email.
     * @param email User's email.
     */
    bool logout(const std::string &email);

    /**
     * Pings the authentication service to check its availability.
     */
    void ping();

    /**
     * Retrieves the current JWT token from the client state.
     * @return The JWT token as a string.
     */
    std::string getJwt() const;

    /**
     * Registers a new user with the provided details.
     * @param email User's email.
     * @param password User's password.
     * @param username Desired username.
     * @param firstName User's first name.
     * @param lastName User's last name.
     * @return true if registration is successful, false otherwise.
     */
    bool registerUser(const std::string &email,
                      const std::string &password,
                      const std::string &username,
                      const std::string &firstName,
                      const std::string &lastName);

    /**
     * Shared client state (authentication, JWT, etc.)
     */
    std::shared_ptr<ClientState> state_;
private:
    HttpGatewayClient &http_;
};
