#include "EnvLoader.h"
#include "dotenv.h"
#include <string>

namespace env
{

    std::string withEnvFilePath(const std::string &filename)
    {
        return "config/" + filename + ".env";
    }

    std::string loadEnv(const std::string &path)
    {
        dotenv::init(withEnvFilePath(path).c_str());
        std::string connStr = "host=" + env::getVar("DB_HOST") + " " +
                              "port=" + env::getVar("DB_PORT") + " " +
                              "dbname=" + env::getVar("DB_NAME") + " " +
                              "user=" + env::getVar("DB_USER") + " " +
                              "password=" + env::getVar("DB_PASSWORD");
        return connStr;
    }

    std::string loadPepperSecret(const std::string &path)
    {
        dotenv::init(withEnvFilePath(path).c_str());
        return env::getVar("PASSWORD_PEPPER");
    }

    std::string getVar(const std::string &key, const std::string &fallback)
    {
        return dotenv::getenv(key.c_str(), fallback);
    }
}
