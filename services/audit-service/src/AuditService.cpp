#include "AuditService.h"
#include <iostream>
#include <thread>
using namespace drogon;

AuditService::AuditService(DbConnection& dbConnection,
                           std::vector<TargetService> targets,
                           trantor::EventLoop* eventLoop)
    : database_(dbConnection),
      targets_(std::move(targets)),
      eventLoop_(eventLoop) {}

void AuditService::ensureSchema() {
    database_.client()->execSqlSync(
        "CREATE TABLE IF NOT EXISTS service_status ("
        "  service      TEXT NOT NULL,"
        "  instance     TEXT NOT NULL,"
        "  last_seen_ts TIMESTAMPTZ NOT NULL DEFAULT TIMESTAMPTZ 'epoch',"
        "  status       TEXT NOT NULL,"
        "  latency_ms   INTEGER NOT NULL DEFAULT 0,"
        "  PRIMARY KEY (service, instance)"
        ");"
    );
    database_.client()->execSqlSync(
        "CREATE TABLE IF NOT EXISTS events ("
        "  id BIGSERIAL PRIMARY KEY,"
        "  ts TIMESTAMPTZ NOT NULL,"
        "  service TEXT NOT NULL,"
        "  instance TEXT NOT NULL,"
        "  level TEXT NOT NULL,"
        "  class TEXT NOT NULL,"
        "  type TEXT NOT NULL,"
        "  correlation_id TEXT,"
        "  data JSONB NOT NULL DEFAULT '{}'::jsonb"
        ");"
    );
}

void AuditService::startScheduler(double intervalSeconds) {
    drogon::app().getLoop()->runEvery(intervalSeconds, [this]() {
        this->refreshOnce();
    });
}

void AuditService::refreshOnce() {
    for (const auto& t : targets_) {
        pingAndRecord(t);
    }
}

void AuditService::pingSingleNow(const std::string& serviceName,
                                 const std::string& baseUrl,
                                 const std::string& healthPath)
{
    TargetService t{serviceName, baseUrl, healthPath};
    pingAndRecord(t);
}

void AuditService::setTargets(std::vector<TargetService> targets) {
    targets_ = std::move(targets);
}

void AuditService::addTarget(TargetService t) {
    targets_.push_back(std::move(t));
}

void AuditService::upsertStatus(const std::string& serviceName,
                                const std::string& instanceId,
                                bool isUp,
                                int latencyMs)
{
    if (isUp) {
        database_.client()->execSqlSync(
            "INSERT INTO service_status(service,instance,last_seen_ts,status,latency_ms) "
            "VALUES($1,$2,NOW(),$3,$4) "
            "ON CONFLICT(service,instance) DO UPDATE SET "
            "  last_seen_ts = NOW(), "
            "  status       = EXCLUDED.status, "
            "  latency_ms   = EXCLUDED.latency_ms;",
            serviceName, instanceId, "UP", latencyMs
        );
    } else {
        database_.client()->execSqlSync(
            "INSERT INTO service_status(service,instance,last_seen_ts,status,latency_ms) "
            "VALUES($1,$2,COALESCE((SELECT last_seen_ts FROM service_status "
            "                        WHERE service=$1 AND instance=$2),"
            "                       TIMESTAMPTZ 'epoch'),"
            "       $3,$4) "
            "ON CONFLICT(service,instance) DO UPDATE SET "
            "  status       = EXCLUDED.status, "
            "  latency_ms   = EXCLUDED.latency_ms, "
            "  last_seen_ts = service_status.last_seen_ts;",
            serviceName, instanceId, "DOWN", latencyMs
        );
    }
}

void AuditService::pingAndRecord(const TargetService& t) {
    auto http = HttpClient::newHttpClient(t.baseUrl, eventLoop_);

    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath(t.healthPath);

    auto start = std::chrono::steady_clock::now();
    auto prom  = std::make_shared<std::promise<std::tuple<bool,int,int>>>();
    auto fut   = prom->get_future();

    http->sendRequest(req, [prom, start](ReqResult r, const HttpResponsePtr& resp) {
        const int latency = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - start).count();
        const int code = resp ? (int)resp->getStatusCode() : -1;
        const bool up  = (r == ReqResult::Ok && resp && resp->getStatusCode() == 200);
        prom->set_value(std::make_tuple(up, code, latency));
    });

    std::thread([this, f = std::move(fut), name = t.serviceName, instance = t.baseUrl]() mutable {
        auto [up, code, latency] = f.get();
        try {
            upsertStatus(name, instance, up, latency);
            std::cout << "[audit] " << name << " - "
                      << (up ? "UP" : "DOWN")
                      << " (" << code << ") "
                      << latency << "ms - "
                      << instance << "\n";
        } catch (const std::exception& e) {
            std::cerr << "[audit] upsert failed: " << e.what() << "\n";
        }
    }).detach();
}