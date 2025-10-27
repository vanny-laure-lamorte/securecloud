#include "AuditRepository.h"
#include <drogon/drogon.h>
using namespace drogon;

void AuditRepository::ensureSchema()
{
    db_.client()->execSqlSync(
        "CREATE TABLE IF NOT EXISTS service_status ("
        "  service      TEXT NOT NULL,"
        "  instance     TEXT NOT NULL,"
        "  last_seen_ts TIMESTAMPTZ NOT NULL DEFAULT TIMESTAMPTZ 'epoch',"
        "  status       TEXT NOT NULL,"
        "  latency_ms   INTEGER DEFAULT 0,"
        "  PRIMARY KEY (service, instance)"
        ");");
}

bool AuditRepository::statusExists(const std::string &serviceName,
                                   const std::string &instanceId)
{
    auto r = db_.client()->execSqlSync(
        "SELECT 1 FROM service_status WHERE service=$1 AND instance=$2;",
        serviceName, instanceId);
    return !r.empty();
}

void AuditRepository::insertStatusUp(const std::string &serviceName,
                                     const std::string &instanceId,
                                     int latencyMs)
{
    db_.client()->execSqlSync(
        "INSERT INTO service_status (service, instance, last_seen_ts, status, latency_ms) "
        "VALUES ($1, $2, NOW(), 'UP', $3);",
        serviceName, instanceId, latencyMs);
}

void AuditRepository::insertStatusDown(const std::string &serviceName,
                                       const std::string &instanceId)
{
    db_.client()->execSqlSync(
        "INSERT INTO service_status (service, instance, last_seen_ts, status, latency_ms) "
        "VALUES ($1, $2, TIMESTAMPTZ 'epoch', 'DOWN', NULL);",
        serviceName, instanceId);
}

void AuditRepository::updateStatusUp(const std::string &serviceName,
                                     const std::string &instanceId,
                                     int latencyMs)
{
    db_.client()->execSqlSync(
        "UPDATE service_status SET "
        "  last_seen_ts = NOW(), "
        "  status       = 'UP', "
        "  latency_ms   = $3 "
        "WHERE service=$1 AND instance=$2;",
        serviceName, instanceId, latencyMs);
}

void AuditRepository::updateStatusDown(const std::string &serviceName,
                                       const std::string &instanceId)
{
    db_.client()->execSqlSync(
        "UPDATE service_status SET "
        "  status     = 'DOWN', "
        "  latency_ms = NULL "
        "WHERE service=$1 AND instance=$2;",
        serviceName, instanceId);
}

std::vector<ServiceStatus> AuditRepository::fetchAllStatuses()
{
    auto r = db_.client()->execSqlSync(
        "SELECT service, instance, status, latency_ms, "
        "       last_seen_ts AT TIME ZONE 'UTC' AS last_seen_utc "
        "FROM service_status "
        "ORDER BY service, instance;");

    std::vector<ServiceStatus> results;
    results.reserve(r.size());
    for (const auto &row : r)
    {
        results.push_back({row["service"].as<std::string>(),
                           row["instance"].as<std::string>(),
                           row["status"].as<std::string>(),
                           row["latency_ms"].as<int>(),
                           row["last_seen_utc"].as<std::string>()});
    }
    return results;
}
