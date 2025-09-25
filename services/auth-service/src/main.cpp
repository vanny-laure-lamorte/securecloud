#include <drogon/drogon.h>

#include "../include/AuthServiceRoutes.h"
#include "../../../common/db/DbConnection.h"
#include "../../../common/Env/EnvLoader.h"
#include "../../../common/HashUtils/HashUtils.h"

int main() {
    using namespace drogon;

    std::string connStr = env::loadEnv("auth-service");
    std::string pepper = env::loadPepperSecret("auth-service");
    DbConnection db = DbConnection(connStr, 2);

    // [Debug] Test HashUtils
    std::string password = "MySecurePassword";
    std::string salt = HashUtils::generateSalt(16);
    std::string hashed = HashUtils::hashPassword(password, salt, pepper);
    std::cout << "Password    : " << password << "\n";
    std::cout << "Salt (base64): " << salt << "\n";
    std::cout << "Hashed      : " << hashed << "\n";
    bool valid = HashUtils::verifyPassword(password, salt, pepper, hashed);
    std::cout << "Verification: " << (valid ? "Success" : "Failure") << "\n";

    app().addListener("0.0.0.0", 8081);

    AuthServiceRoutes::registerRoutes();

    app().run();
    return 0;
}
