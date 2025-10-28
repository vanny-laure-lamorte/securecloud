#pragma once
#include "AuditRepository.h"
#include "AuditService.h"
#include "AuditRepository.h"
#include "AuditService.h"

#include <drogon/drogon.h>
#include <drogon/drogon.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <json/json.h>
using namespace drogon;
using namespace drogon;

class AuditController
{
public:
/**
 * Constructor
 * @param db Database connection
 * @param loop Event loop
 */
    AuditController(DbConnection& db, trantor::EventLoop* loop);

    /**
     * Handle ping request
     * @param req HTTP request
     */
    drogon::HttpResponsePtr handlePing(const drogon::HttpRequestPtr& req);

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
    /**
     * Parse JSON from HTTP request
     * @param req HTTP request
     * @param out Output JSON value
     * @return true if parsing was successful, false otherwise
     */
    bool parseJson(const drogon::HttpRequestPtr& req, Json::Value& out);

    AuditRepository repo_;
    AuditService service_;
};