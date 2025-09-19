#include <drogon/drogon.h>
#include "AuditDb.h"
#include "Routes.h"
#include "Refresh.h"
#include <iostream>

int main() {
    audit::connectPg();
    audit::runMigrations();
    audit::registerRoutes();

    drogon::app().addListener("0.0.0.0", 8083);

    audit::schedulePeriodicRefresh(10.0);

    std::cout << "[audit-service] listening on http://127.0.0.1:8083\n";
    drogon::app().run();
    return 0;
}
