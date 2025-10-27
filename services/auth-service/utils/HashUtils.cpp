#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <stdexcept>
#include <vector>
#include <cstring>

#include "HashUtils.h"
#include "../../../common/Env/EnvLoader.h"

namespace HashUtils {

    std::string base64Encode(const unsigned char* data, size_t length) {
        BIO* bio, * b64;
        BUF_MEM* bufferPtr;

        b64 = BIO_new(BIO_f_base64());
        bio = BIO_new(BIO_s_mem());
        b64 = BIO_push(b64, bio);

        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

        BIO_write(b64, data, static_cast<int>(length));
        BIO_flush(b64);
        BIO_get_mem_ptr(b64, &bufferPtr);

        std::string result(bufferPtr->data, bufferPtr->length);
        BIO_free_all(b64);
        return result;
    }

    std::string hashPassword(const std::string& password, const std::string& salt) {

        std::string pepper = env::loadPepperSecret("auth-service");
        std::string combined = salt + password + pepper;

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(combined.c_str()), combined.size(), hash);

        static const char hex_chars[] = "0123456789abcdef";
        std::string hexStr;
        hexStr.reserve(2 * SHA256_DIGEST_LENGTH);
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            hexStr.push_back(hex_chars[(hash[i] >> 4) & 0xF]);
            hexStr.push_back(hex_chars[hash[i] & 0xF]);
        }

        return hexStr;
    }

    bool verifyPassword(const std::string& password, const std::string& salt, const std::string& hashed) {
        return hashPassword(password, salt) == hashed;
    }

    std::string generateSalt(int length) {
        if (length <= 0) {
            throw std::invalid_argument("Salt length must be greater than 0");
        }

        std::vector<unsigned char> buffer(length);
        if (RAND_bytes(buffer.data(), length) != 1) {
            throw std::runtime_error("Failed to generate random bytes");
        }

        return base64Encode(buffer.data(), buffer.size());
    }
}
