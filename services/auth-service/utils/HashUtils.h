#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <string>

namespace HashUtils {

    /**
     * @brief Hashes a password using the provided salt and pepper.
     * @param password The password to hash.
     * @param salt The salt to use in hashing.
     * @return The resulting hashed password as a hexadecimal string.
     */
    std::string hashPassword(const std::string& password, const std::string& salt);

    /**
     * @brief Verifies a password against a given hash using the provided salt and pepper.
     * @param password The password to verify.
     * @param salt The salt used in the original hashing.
     * @param hashed The hashed password to compare against.
     * @return True if the password matches the hash, false otherwise.
     */
    bool verifyPassword(const std::string& password, const std::string& salt, const std::string& hashed);

    /**
     * @brief Generates a random salt of the specified length
     * @param length The length of the salt to generate.
     * @return A base64-encoded string representing the generated salt.
     */
    std::string generateSalt(int length);
}

#endif // HASHUTILS_H
