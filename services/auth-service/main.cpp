#include <drogon/drogon.h>

#include "routes/AuthServiceRoutes.h"
#include "common/db/DbConnection.h"
#include "common/Env/EnvLoader.h"
#include "utils/HashUtils.h"

int main()
{
    using namespace drogon;
    std::string connStr = env::loadEnv("auth");
    std::cout << std::endl
              << "====== [AUTH SERVICE connection string] ======" << std::endl;
    std::cout << connStr << std::endl;
    std::cout << "===============================================" << std::endl
              << std::endl;
    DbConnection db = DbConnection(connStr, 2);

    app().addListener("0.0.0.0", 8081);
    AuthServiceRoutes authRoutes(db);
    authRoutes.registerRoutes();

    app().run();

    return 0;
}
