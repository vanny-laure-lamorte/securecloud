#include <drogon/drogon.h>
#include <common/db/DbConnection.h>
#include "AuditService.h"
#include "AuditRoutes.h"
#include <iostream>

int main() {
    DbConnection db = DbConnection("host=DB_HOST port=DB_PORT dbname=DB_NAME user=DB_USER password=DB_PASSWORD", 2);

    std::vector<TargetService> targets = {
        {"auth",      "http://127.0.0.1:8081", "/auth/ping"},
        {"messaging", "http://127.0.0.1:8082", "/messaging/ping"},
    };

    AuditService audit(db, std::move(targets), drogon::app().getLoop());
    audit.ensureSchema();
    audit.startScheduler(10.0);

    AuditRoutes routes(db, audit);
    routes.registerAll();

    drogon::app().addListener("0.0.0.0", 8083);
    std::cout << "[audit-service] scheduler started (10s)\n";

    drogon::app().run();
    return 0;
}
