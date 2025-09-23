#include "AuditDb.h"
#include <cstdlib>
#include <iostream>

using namespace drogon::orm;

namespace audit {

drogon::orm::DbClientPtr gDb;

void connectPg() {
    const char* env = ::getenv("PG_CONN");
    std::string conn = env ? env
        : "host=127.0.0.1 port=5432 dbname=audit_service user=postgres password=Lazlo";
    gDb = DbClient::newPgClient(conn, 2);
}

void runMigrations() {
    gDb->execSqlSync(
        "CREATE TABLE IF NOT EXISTS events ("
        "  id BIGSERIAL PRIMARY KEY,"
        "  ts TIMESTAMPTZ NOT NULL,"
        "  service TEXT NOT NULL,"
        "  instance TEXT NOT NULL,"
        "  level TEXT NOT NULL,"
        "  class TEXT NOT NULL,"
        "  type TEXT NOT NULL,"
        "  correlation_id TEXT,"
        "  data JSONB NOT NULL DEFAULT '{}'::jsonb"
        ");"
    );
    gDb->execSqlSync(
        "CREATE TABLE IF NOT EXISTS service_status ("
        "  service      TEXT NOT NULL,"
        "  instance     TEXT NOT NULL,"
        "  last_seen_ts TIMESTAMPTZ NOT NULL DEFAULT TIMESTAMPTZ 'epoch',"
        "  status       TEXT NOT NULL,"
        "  latency_ms   INTEGER NOT NULL DEFAULT 0,"
        "  PRIMARY KEY (service, instance)"
        ");"
    );
}

void upsertStatus(const std::string& service,
                  const std::string& instance,
                  const std::string& status,
                  int latency_ms,
                  const std::string& seen_ts_opt) {
    if (status == "UP") {
        if (!seen_ts_opt.empty()) {
            gDb->execSqlSync(
                "INSERT INTO service_status(service,instance,last_seen_ts,status,latency_ms) "
                "VALUES($1,$2,$3::timestamptz,$4,$5) "
                "ON CONFLICT(service,instance) DO UPDATE SET "
                "  last_seen_ts = EXCLUDED.last_seen_ts, "
                "  status       = EXCLUDED.status, "
                "  latency_ms   = EXCLUDED.latency_ms;",
                service, instance, seen_ts_opt, status, latency_ms
            );
        } else {
            gDb->execSqlSync(
                "INSERT INTO service_status(service,instance,last_seen_ts,status,latency_ms) "
                "VALUES($1,$2,NOW(),$3,$4) "
                "ON CONFLICT(service,instance) DO UPDATE SET "
                "  last_seen_ts = NOW(), "
                "  status       = EXCLUDED.status, "
                "  latency_ms   = EXCLUDED.latency_ms;",
                service, instance, status, latency_ms
            );
        }
    } else {
        gDb->execSqlSync(
            "INSERT INTO service_status(service,instance,last_seen_ts,status,latency_ms) "
            "VALUES($1,$2,COALESCE((SELECT last_seen_ts FROM service_status "
            "                        WHERE service=$1 AND instance=$2),"
            "                       TIMESTAMPTZ 'epoch'),"
            "       $3,$4) "
            "ON CONFLICT(service,instance) DO UPDATE SET "
            "  status       = EXCLUDED.status, "
            "  latency_ms   = EXCLUDED.latency_ms, "
            "  last_seen_ts = service_status.last_seen_ts;",
            service, instance, status, latency_ms
        );
    }
}

}