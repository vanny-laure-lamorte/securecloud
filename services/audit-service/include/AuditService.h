#pragma once
#include "AuditRepository.h"

#include <drogon/drogon.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
using namespace drogon;

struct TargetService
{
    std::string serviceName;
    std::string baseUrl;
    std::string healthPath;
};

class AuditService
{
public:
    /**
     * Constructor
     * @param repo Audit repository
     * @param eventLoop Event loop
     */
    AuditService(AuditRepository &repo,
                 trantor::EventLoop *eventLoop);

    /**
     * Start periodic scheduler to ping services
     * @param intervalSeconds Interval in seconds
     */
    void startScheduler(double intervalSeconds);

    /**
     * Perform a single refresh of all services statuses
     */
    void refreshOnce();

    /**
     * Ping a single service immediately and record its status
     * @param serviceName Service name
     */
    void pingSingleNow(const std::string &serviceName);

    /**
     * Set the list of target services to monitor
     * @param targets Vector of TargetService
     */
    void setTargets(std::vector<TargetService> targets);

    /**
     * Add a target service to monitor
     * @param target TargetService to add
     */
    void addTarget(TargetService target);

    /**
     * Get statuses for a specific service
     * @param serviceName Service name
     * @return Vector of ServiceStatus
     */
    std::vector<ServiceStatus> getStatusesByService(const std::string& serviceName) const;

private:
    /**
     * Ping a target service and record its status
     * @param t TargetService to ping
     */
    void pingAndRecord(const TargetService &t);

    /**
     * Find a target service by its name
     * @param name Service name
     * @return Optional TargetService if found
     */
    std::optional<TargetService> findTargetByName(const std::string &name) const;

    AuditRepository &repo_;
    std::vector<TargetService> targets_;
    trantor::EventLoop *eventLoop_;
};