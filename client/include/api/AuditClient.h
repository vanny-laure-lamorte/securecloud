#pragma once

#include "core/ClientState.h"
#include "core/HttpGatewayClient.h"
#include <string>

class AuditClient
{
public:
    /**
     * Audit Client constructor.
     * @param http Reference to the HTTP gateway client for making requests.
    */
    explicit AuditClient(HttpGatewayClient &http);

    /**
     * Pings the audit service to check its availability.
     * @param serviceName The name of the service to ping.
     */
    void pingService(const std::string &serviceName);

    /**
     * Retrieves the status of a specific service.
     * @param serviceName The name of the service whose status is requested.
     */
    void getServiceStatus(const std::string &serviceName);

    /**
     * Retrieves the status of all services.
     */
    void getAllServicesStatus();

    /**
     * Refreshes the list of services from the audit service.
     */
    void refreshServices();

private:
    HttpGatewayClient &http_;
};
