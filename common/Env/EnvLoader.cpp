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
        if (service != "auth" && service != "audit" && service != "messaging") {
            dotenv::init(withEnvFilePath(service).c_str());
            std::string connStr = "host=" + env::getVar("DB_HOST") + " " +
                                  "port=" + env::getVar("DB_PORT") + " " +
                                  "dbname=" + env::getVar("DB_NAME") + " " +
                                  "user=" + env::getVar("DB_USER") + " " +
                                  "password=" + env::getVar("DB_PASSWORD");
            return connStr;
        } else{
            std::string db_password = loadSecret(service + "_db_password");
            std::string upper = service;
            std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
            std::string connStr = "host=" + getVar(upper + "_DB_HOST", "db") + " " +
                                "port=" + getVar(upper +  "_DB_PORT", "5432") + " " +
                                "dbname=" + getVar(upper + "_DB_NAME", "5432") + " " +
                                "user=" + getVar(upper + "_DB_USER", "5432") + " " +
                                "password=" + db_password;
            return connStr;
        }
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