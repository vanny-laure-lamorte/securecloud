#pragma once
#include <drogon/orm/DbClient.h>
#include <string>

namespace audit {
extern drogon::orm::DbClientPtr gDb;

void connectPg();
void runMigrations();

void upsertStatus(const std::string& service,
                  const std::string& instance,
                  const std::string& status,
                  int latency_ms,
                  const std::string& seen_ts_opt
);
}
