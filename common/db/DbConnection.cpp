#include "DbConnection.h"

using namespace drogon::orm;

DbConnection::DbConnection(const std::string& connectionString, unsigned int poolSize) {
    client_ = DbClient::newPgClient(connectionString, poolSize);
}
