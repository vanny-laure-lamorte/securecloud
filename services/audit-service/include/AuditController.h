#pragma once
#include "AuditRepository.h"
#include "AuditService.h"

#include <drogon/drogon.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <json/json.h>
using namespace drogon;

class AuditController
{
public:
    /**
     * Constructor
     * @param db Database connection
     * @param loop Event loop
     */
    AuditController(DbConnection &db, trantor::EventLoop *loop);

    /**
     * Handle ping for a single service
     * @param req HTTP request
     * @param json JSON payload containing service name
     */
    drogon::HttpResponsePtr handleServicePing(const drogon::HttpRequestPtr &req);

    /**
     * Handle list of all services and their statuses
     * @return HTTP response
     */
    drogon::HttpResponsePtr handleList();

    /**
     * Handle refresh of all services statuses
     * @return HTTP response
     */

    /**
     * Handle refresh of all services statuses
     * @return HTTP response
     */
    drogon::HttpResponsePtr handleRefresh();

    /**
     * Handle fetching status of a single service
     * @param req HTTP request
     * @return HTTP response
     */
    drogon::HttpResponsePtr getOneServiceStatus(const drogon::HttpRequestPtr &req);

private:
    AuditRepository repo_;
    AuditService service_;

    /**
     * Parse JSON from HTTP request
     * @param req HTTP request
     * @param out Output JSON value
     * @return true if parsing was successful, false otherwise
     */
    bool parseJson(const drogon::HttpRequestPtr &req, Json::Value &out);

    /**
     * Extract service name from HTTP request JSON payload
     * @param req HTTP request
     * @param parsedJsonOpt Optional output parameter to receive parsed JSON
     * @return Extracted service name, or empty string if not found
     */
    std::string extractServiceName(const drogon::HttpRequestPtr &req, Json::Value *parsedJsonOpt);
};