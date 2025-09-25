#include <drogon/drogon.h>
#include "../include/AuthServiceRoutes.h"
#include "../../../common/HashUtils/HashUtils.h"

int main() {
    using namespace drogon;

    // [Debug] Test HashUtils
    std::string password = "MySecurePassword";
    std::string salt = HashUtils::generateSalt(16);
    std::string pepper = "SecretPepperTest";
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
