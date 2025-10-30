#include "EnvLoader.h"
#include "dotenv.h"
#include <string>
#include <fstream>
#include <sstream>

namespace env
{

    std::string withEnvFilePath(const std::string &service)
    {
        return "config/" + service + "-service.env";
    }

    std::string loadEnv(const std::string &service)
    {
        const char *dbHostEnv = std::getenv("DB_HOST");
        if (dbHostEnv != nullptr)
        {
            std::string host = std::getenv("DB_HOST") ? std::getenv("DB_HOST") : "";
            std::string port = std::getenv("DB_PORT") ? std::getenv("DB_PORT") : "";
            std::string name = std::getenv("DB_NAME") ? std::getenv("DB_NAME") : "";
            std::string user = std::getenv("DB_USER") ? std::getenv("DB_USER") : "";
            std::string pass = readPassword();

            return std::string("host=") + host +
                   " port=" + port +
                   " dbname=" + name +
                   " user=" + user +
                   " password=" + pass;
        }

        dotenv::init(withEnvFilePath(service).c_str());
        return "host=" + env::getVar("DB_HOST") + " " +
               "port=" + env::getVar("DB_PORT") + " " +
               "dbname=" + env::getVar("DB_NAME") + " " +
               "user=" + env::getVar("DB_USER") + " " +
               "password=" + env::getVar("DB_PASSWORD");
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

    std::string readPassword()
    {
        if (const char *pwFile = std::getenv("DB_PASSWORD_FILE"))
        {
            std::ifstream in(pwFile);
            std::stringstream buf;
            buf << in.rdbuf();
            return buf.str();
        }

        if (const char *pw = std::getenv("DB_PASSWORD"))
        {
            return std::string(pw);
        }
        return "";
    }
}