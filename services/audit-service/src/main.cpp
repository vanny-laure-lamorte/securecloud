#include "common/db/DbConnection.h"
#include "common/Env/EnvLoader.h"
#include "AuditRoutes.h"
#include "AuditService.h"

#include <drogon/drogon.h>
#include <iostream>

int main() {
    std::string connStr = env::loadEnv("audit");
    DbConnection db(connStr, 2);

    AuditRoutes routes(db, drogon::app().getLoop());
    routes.registerAll();

    drogon::app().addListener("0.0.0.0", 8083);
    drogon::app().run();
    return 0;
}