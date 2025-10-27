#include <drogon/drogon.h>
#include <iostream>

#include <common/db/DbConnection.h>
#include "AuditRoutes.h"
#include "AuditService.h"
#include "common/Env/EnvLoader.h"

int main() {
    std::string connStr = env::loadEnv("audit-service");
    DbConnection db(connStr, 2);

    AuditRoutes routes(db, drogon::app().getLoop());
    routes.registerAll();

    drogon::app().addListener("0.0.0.0", 8083);
    std::cout << "[audit-service] started\n";
    drogon::app().run();
    return 0;
}
