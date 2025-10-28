#pragma once
#include "common/db/DbConnection.h"

#include <drogon/drogon.h>
#include <vector>
#include <string>
using namespace drogon;

struct ServiceStatus {
    std::string service;
    std::string instance;
    std::string status;
    int         latencyMs{};
    std::string lastSeenUtc;
};

class AuditRepository {
public:
    /**
     * Constructor
     * @param db Database connection
     */
    explicit AuditRepository(DbConnection& db) : db_(db) {}

    /**
     * Ensure database schema exists
     */
    void ensureSchema();

    /**
     * Check if status entry exists
     * @param serviceName Service name
     * @param instanceId Instance identifier
     * @return true if exists, false otherwise
     */
    bool statusExists(const std::string& serviceName, const std::string& instanceId);

    /**
     * Fetch all service statuses
     * @return Vector of ServiceStatus
     */
    std::vector<ServiceStatus> fetchAllStatuses();

    /**
     * Insert or update status to UP
     * @param serviceName Service name
     * @param instanceId Instance identifier
     * @param latencyMs Latency in milliseconds
     */
    void insertStatusUp(const std::string& serviceName,
                        const std::string& instanceId,
                        int latencyMs);

    /**
     * Insert or update status as DOWN
     * @param serviceName Service name
     * @param instanceId Instance identifier
     */
    void insertStatusDown(const std::string& serviceName,
                          const std::string& instanceId);

    /**
     * Update status to UP
     * @param serviceName Service name
     * @param instanceId Instance identifier
     * @param latencyMs Latency in milliseconds
     */
    void updateStatusUp(const std::string& serviceName,
                        const std::string& instanceId,
                        int latencyMs);
    /**
     * Update status to DOWN
     * @param serviceName Service name
     * @param instanceId Instance identifier
     */
    void updateStatusDown(const std::string& serviceName,
                          const std::string& instanceId);

private:
    DbConnection& db_;
};