#pragma once
#include <string>

namespace env {

    /**
     * @brief Constructs the full path to the .env file given a filename.
     * @param filename The base name of the .env file (without path and extension)
     */
    std::string withEnvFilePath(const std::string& filename);

    /**
     * @brief Loads environment variables from a specified .env file
     * to be able to constructs a database connection string.
     * @param path The path to the .env file from config folder
     */
    std::string loadEnv(const std::string& path = ".env");

    /**
     * @brief Loads the pepper secret
     * @param path The path to the .env file from config folder
     *
     */
    std::string loadPepperSecret(const std::string& path = ".env");

    /**
     * @brief Retrieves the value of an environment variable, with an optional fallback.
     */
    std::string getVar(const std::string& key, const std::string& fallback = "");
}
