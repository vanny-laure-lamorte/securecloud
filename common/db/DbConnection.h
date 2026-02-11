#pragma once
#include <drogon/orm/DbClient.h>
#include <string>

class DbConnection {
public:
    /**
     * @brief Construct using a full libpq connection string.
     * @param connectionString e.g. "host=127.0.0.1 port=5432 dbname=... user=... password=..."
     * @param poolSize number of connections in the pool (default: 2)
     * @throws std::exception if client cannot be created.
     */
    explicit DbConnection(const std::string& connectionString, unsigned int poolSize = 2);

    drogon::orm::DbClientPtr client() const { return client_; }

private:
    drogon::orm::DbClientPtr client_;
};
