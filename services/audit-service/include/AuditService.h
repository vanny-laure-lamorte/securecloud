#pragma once
#include <string>
#include <vector>
#include <drogon/drogon.h>
#include <common/db/DbConnection.h>

/** A target service to be pinged. */
struct TargetService
{
    std::string serviceName; ///< e.g. "auth"
    std::string baseUrl;     ///< e.g. "http://127.0.0.1:8081"
    std::string healthPath;  ///< e.g. "/auth/ping"
};

class AuditService
{
public:
    AuditService(DbConnection &dbConnection,
                 std::vector<TargetService> targets,
                 trantor::EventLoop *eventLoop);

    /** Create schema: service_status + events (if not exists). */
    void ensureSchema();

    /** Schedule periodic pings every intervalSeconds. */
    void startScheduler(double intervalSeconds);

    /** Ping all configured targets now (non-blocking). */
    void refreshOnce();

    /** Alias for refreshOnce(). */
    void pingAllNow() { refreshOnce(); }

    /** Ping a single service now (non-blocking). */
    void pingSingleNow(const std::string &serviceName,
                       const std::string &baseUrl,
                       const std::string &healthPath);

    /** Replace target list. */
    void setTargets(std::vector<TargetService> targets);
    /** Append one target. */
    void addTarget(TargetService t);

private:
    DbConnection &database_;
    std::vector<TargetService> targets_;
    trantor::EventLoop *eventLoop_;

    /**
     * @brief Insert or update service status in DB.
     * @param serviceName Name of the service (e.g. "auth")
     * @param instanceId Unique instance identifier (e.g. base URL)
     * @param isUp true if service is reachable (HTTP 200), false otherwise
     * @param latencyMs Measured latency in milliseconds
     */
    void upsertStatus(const std::string &serviceName,
                      const std::string &instanceId,
                      bool isUp,
                      int latencyMs);

    /**
     * @brief Ping a target service and record its status in DB.
     * @param t TargetService struct with service details
     */
    void pingAndRecord(const TargetService &t);
};
