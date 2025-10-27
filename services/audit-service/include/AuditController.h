#pragma once

#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <json/json.h>

#include "AuditRepository.h"
#include "AuditService.h"

class AuditController {
public:
    AuditController(DbConnection& db, trantor::EventLoop* loop);

    drogon::HttpResponsePtr handlePing(const drogon::HttpRequestPtr& req);
    drogon::HttpResponsePtr handleList();
    drogon::HttpResponsePtr handleRefresh();

private:
    bool parseJson(const drogon::HttpRequestPtr& req, Json::Value& out);

    AuditRepository repo_;
    AuditService service_;
};
