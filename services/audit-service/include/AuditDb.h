#pragma once
#include <drogon/orm/DbClient.h>
#include <string>

namespace audit {
extern drogon::orm::DbClientPtr gDb;

// Connect to PostgreSQL and run migrations
void connectPg();
// Create tables if they do not exist
void runMigrations();

// Update or Insert service status
void upsertStatus(const std::string& service,
                  const std::string& instance,
                  const std::string& status,
                  int latency_ms,
                  const std::string& seen_ts_opt
);
}
