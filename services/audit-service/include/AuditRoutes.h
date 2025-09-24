#pragma once
#include <iostream>
#include <common/db/DbConnection.h>
#include <drogon/drogon.h>
#include <json/json.h>
#include "AuditService.h"

class AuditRoutes
{
public:
    /**
     * @brief Construct with DB connection and AuditService instance.
     * @param db Database connection (for queries)
     * @param service AuditService instance (for pings)
     */
    AuditRoutes(DbConnection &db, AuditService &service);

    /**
     * @brief Register all HTTP routes/handlers.
     */
    void registerAll();

private:
    DbConnection &db_;
    AuditService &service_;

    /**
     * @brief Parse JSON body from request into Json::Value.
     * @param req Input HTTP request
     * @param out Output Json::Value (if parsing successful)
     * @return true if parsing was successful, false otherwise
     */
    static bool parseJson(const drogon::HttpRequestPtr &req, Json::Value &out);
};
