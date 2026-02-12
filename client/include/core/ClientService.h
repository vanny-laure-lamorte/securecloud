#pragma once

#include "api/AuthClient.h"
#include "api/AuditClient.h"
#include "api/MessagingApiClient.h"
#include "core/WsGatewayClient.h"
#include "core/ClientState.h"
#include <string>
#include <QMetaObject>

class QObject;
class AuthClient;
class AuditClient;
class MessagingApiClient;
class WsGatewayClient;

class ClientService
{
public:
    /**
     * ClientService constructor.
     * @param auth Reference to the AuthClient for authentication operations.
     * @param audit Reference to the AuditClient for audit operations.
     * @param messaging Reference to the MessagingApiClient for messaging operations.
     * @param wsClient Pointer to the WsGatewayClient for WebSocket operations.
     * @param qtContext Pointer to a QObject that can be used as a context for Qt signal-slot connections.
     */
    ClientService(AuthClient& auth,
                  AuditClient& audit,
                  MessagingApiClient& messaging,
                  WsGatewayClient* wsClient,
                  QObject* qtContext);

    /**
     * Attempts to log in a user with the provided email and password.
     * On successful login, establishes a WebSocket connection using the obtained JWT.
     * @param email The email of the user attempting to log in.
     * @param password The password of the user attempting to log in.
     * @return true if login is successful, false otherwise.
     */
    bool login(const std::string& email, const std::string& password);

    /**
     * Logs out the currently authenticated user.
     * @return true if logout is successful, false otherwise.
     */
    bool logout();

    /**
     * Registers a new user with the provided details.
     * @param email The email of the user to register.
     * @param password The password of the user to register.
     * @param username The desired username of the user to register.
     * @param firstName The first name of the user to register.
     * @param lastName The last name of the user to register.
     * @param dob The date of birth of the user to register.
     * @return true if registration is successful, false otherwise.
     */
    bool registerUser(const std::string& email,
                      const std::string& password,
                      const std::string& username,
                      const std::string& firstName,
                      const std::string& lastName,
                      const std::string& dob);
    /**
     * Pings the authentication service to check its availability.
     * This can be used to verify that the auth service is responsive and to keep the connection alive.
    */
    void pingAuth();

    /**
     * Retrieves the status of all services from the audit service.
     * This can be used to check the health of the various backend services.
     */
    void getServicesStatus();

    /**
     * Refreshes the list of services from the audit service.
     * This can be used to get the latest status of services.
     */
    void refreshServices();

    /**
     * Pings a specific service by name using the audit service.
     * @param serviceName The name of the service to ping (e.g., "auth", "messaging").
     * This can be used to check the responsiveness of a specific service.
     */
    void pingService(const std::string& serviceName);

    /**
     * Retrieves all messages for the authenticated user, including personal and group messages.
     * This can be used to populate the user's message history in the UI.
     */
    void getAllMessages();

    /**
     * Sends a personal message to another user via the WebSocket connection.
     * @param targetUserId The user ID of the recipient.
     * @param message The message content to send.
     * This can be used to send real-time messages to other users.
     */
    void sendPersonal(int targetUserId, const std::string& message);

    /**
     * Sends a message to a group via the WebSocket connection.
     * @param targetGroupId The group ID of the recipient group.
     * @param message The message content to send.
     * This can be used to send real-time messages to groups the user is part of.
     */
    void sendGroup(int targetGroupId, const std::string& message);

    /**
     * Checks if there is a currently authenticated user.
     * @return true if a user is authenticated, false otherwise.
     */
    bool isAuthenticated() const;

    /**
     * Retrieves the user ID of the currently authenticated user.
     * @return The user ID if authenticated, or -1 if no user is authenticated.
     */
    int userId() const;

    /**
     * Retrieves the email of the currently authenticated user.
     * @return The email if authenticated, or an empty string if no user is authenticated.
     */
    std::string email() const;

    /**
     * Retrieves the groups of authenticated user.
     * @return A vector of maps, where each map represents a group with its details (e.g., group ID, group name).
     */
    QVector<QPair <int, QString>> getGroups();

    /**
     * Retrieves the messages for a specific group or personal conversation.
     * @param id The ID of the group or user to retrieve messages for.
     * @return A vector of maps, where each map represents a message with its details (e.g., message ID, content, sender ID).
     */
    QVector<QPair <int, QString>> getMessages(int id, const std::string& type);

private:
    /**
     * Establishes a WebSocket connection using the provided JWT for authentication.
     * This should be called on the Qt thread to ensure that any signals emitted by the
     * WsGatewayClient are properly handled in the Qt event loop.
     */
    void wsConnectWithJwtOnQtThread(const std::string& jwt);

    /**
     * Sends a personal message via the WebSocket connection on the Qt thread.
     * @param fromUserId The user ID of the sender (current authenticated user).
     * @param targetUserId The user ID of the recipient.
     * @param msg The message content to send.
     * This should be called on the Qt thread to ensure that any signals emitted by the
     * WsGatewayClient are properly handled in the Qt event loop.
     */
    void wsSendPersonalOnQtThread(int fromUserId,
                                  int targetUserId,
                                  const std::string& msg);

    /**
     * Sends a group message via the WebSocket connection on the Qt thread.
     * @param fromUserId The user ID of the sender (current authenticated user).
     * @param groupId The group ID of the recipient group.
     * @param msg The message content to send.
     * This should be called on the Qt thread to ensure that any signals emitted by the
     * WsGatewayClient are properly handled in the Qt event loop.
     */
    void wsSendGroupOnQtThread(int fromUserId,
                               int groupId,
                               const std::string& msg);

private:
    AuthClient& auth_;
    AuditClient& audit_;
    MessagingApiClient& messaging_;
    WsGatewayClient* wsClient_ = nullptr;

    QObject* qtContext_ = nullptr;
};
