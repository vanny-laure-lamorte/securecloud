#include "api/AuditClient.h"

#include <drogon/drogon.h>
#include <json/json.h>
#include <iostream>
#include <memory>

using namespace drogon;

AuditClient::AuditClient(HttpGatewayClient &http)
    : http_(http)
{
}

void AuditClient::pingService(const std::string &serviceName)
{
    Json::Value payload;
    payload["service"] = serviceName;

    auto req = HttpRequest::newHttpJsonRequest(payload);
    req->setMethod(Post);
    req->setPath("/audit/service_ping");

    auto [code, respBody] = http_.send(req);
    std::cout << "[AuditClient] " << code << " | " << respBody << "\n";
}

void AuditClient::getServiceStatus(const std::string &serviceName)
{
    Json::Value payload;
    payload["service"] = serviceName;

    auto req = HttpRequest::newHttpJsonRequest(payload);
    req->setMethod(Get);
    req->setPath("/audit/get_one_service_status");

    auto [code, respBody] = http_.send(req);
    if (code != 200)
    {
        std::cout << "[AuditClient] " << code << " | " << respBody << "\n";
        return;
    }

    Json::Value obj;
    Json::CharReaderBuilder b;
    std::string errs;
    std::unique_ptr<Json::CharReader> r(b.newCharReader());
    if (!r->parse(respBody.data(), respBody.data() + respBody.size(), &obj, &errs))
    {
        std::cout << "[AuditClient] invalid JSON: " << errs << "\n";
        return;
    }

    const std::string svc  = obj.get("service", "").asString();
    const std::string inst = obj.get("instance", "").asString();
    const std::string stat = obj.get("status", "").asString();
    const int         lat  = obj.get("latency_ms", 0).asInt();
    const std::string ls   = obj.get("last_seen_utc", "").asString();

    std::cout << "[AuditClient] "
              << svc << " - " << stat << " - " << lat << "ms - "
              << inst << " Last seen - " << ls << "\n";
}

void AuditClient::getAllServicesStatus()
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/audit/services");

    auto [code, body] = http_.send(req);
    if (code != 200)
    {
        std::cout << "[AuditClient] " << code << " | " << body << "\n";
        return;
    }

    Json::Value arr;
    Json::CharReaderBuilder b;
    std::string errs;
    std::unique_ptr<Json::CharReader> r(b.newCharReader());
    if (!r->parse(body.data(), body.data() + body.size(), &arr, &errs) || !arr.isArray())
    {
        std::cout << "[AuditClient] invalid JSON\n";
        return;
    }

    for (const auto &it : arr)
    {
        const std::string svc  = it.get("service", "").asString();
        const std::string inst = it.get("instance", "").asString();
        const std::string stat = it.get("status", "").asString();
        const int         lat  = it.get("latency_ms", 0).asInt();
        const std::string ls   = it.get("last_seen_utc", "").asString();

        std::cout << svc << " - " << stat << " - " << lat << "ms - "
                  << inst << " Last seen - " << ls << "\n";
    }
}

void AuditClient::refreshServices()
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Post);
    req->setPath("/audit/services");

    auto [code, body] = http_.send(req);
    if (code != 200)
    {
        std::cout << "[AuditClient] " << code << " | " << body << "\n";
        return;
    }

    std::cout << "[AuditClient] " << code << " | Services status refreshed\n";
}
