#pragma once
#include <vector>
#include <string>

#include <common/db/DbConnection.h>

struct ServiceStatus {
    std::string service;
    std::string instance;
    std::string status;
    int         latencyMs{};
    std::string lastSeenUtc;
};

class AuditRepository {
public:
    explicit AuditRepository(DbConnection& db) : db_(db) {}

    void ensureSchema();

    bool statusExists(const std::string& serviceName, const std::string& instanceId);
    std::vector<ServiceStatus> fetchAllStatuses();

    void insertStatusUp(const std::string& serviceName,
                        const std::string& instanceId,
                        int latencyMs);
    void insertStatusDown(const std::string& serviceName,
                          const std::string& instanceId);

    void updateStatusUp(const std::string& serviceName,
                        const std::string& instanceId,
                        int latencyMs);
    void updateStatusDown(const std::string& serviceName,
                          const std::string& instanceId);

private:
    DbConnection& db_;
};
