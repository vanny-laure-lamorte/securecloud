#pragma once
#include "AuditController.h"
#include "common/db/DbConnection.h"

#include <drogon/drogon.h>
using namespace drogon;

class AuditRoutes {
public:
    /**
     * Constructor
     * @param db Database connection
     * @param loop Event loop
     */
    AuditRoutes(DbConnection& db, trantor::EventLoop* loop);

    /**
     * Register all routes
     */
    void registerAll();

private:
    AuditController controller_;
};