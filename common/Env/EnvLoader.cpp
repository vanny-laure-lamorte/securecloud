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
        std::string authDbName = loadSecret("auth_db_name");

        if (authDbName.empty()) {

            dotenv::init(withEnvFilePath(path).c_str());
            std::string connStr = "host=" + env::getVar("DB_HOST") + " " +
                                  "port=" + env::getVar("DB_PORT") + " " +
                                  "dbname=" + env::getVar("DB_NAME") + " " +
                                  "user=" + env::getVar("DB_USER") + " " +
                                  "password=" + env::getVar("DB_PASSWORD");
            return connStr;
        } else{
            std::string db_name = loadSecret("auth_db_name");
            std::string db_user = loadSecret("auth_db_user");
            std::string db_password = loadSecret("auth_db_password");

            std::string connStr = "host=" + getVar("DB_HOST", "db") + " " +
                                "port=" + getVar("DB_PORT", "5432") + " " +
                                "dbname=" + db_name + " " +
                                "user=" + db_user + " " +
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

    std::string loadSecret(const std::string &secretName)
    {
        std::ifstream file("/run/secrets/" + secretName);
        std::string content;
        if (file.is_open()) {
            std::getline(file, content);
        }
        return content;
    }

}
