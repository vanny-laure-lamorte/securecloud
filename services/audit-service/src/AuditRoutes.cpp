#include "AuditRoutes.h"
using namespace drogon;

AuditRoutes::AuditRoutes(DbConnection& db, AuditService& service)
    : db_(db), service_(service) {}

bool AuditRoutes::parseJson(const HttpRequestPtr& req, Json::Value& out) {
    Json::CharReaderBuilder b; std::string errs;
    const auto buf = req->getBody();
    std::string s(buf.data(), buf.size());
    auto r = std::unique_ptr<Json::CharReader>(b.newCharReader());
    return r->parse(s.data(), s.data()+s.size(), &out, &errs);
}

void AuditRoutes::registerAll() {
    app().registerHandler(
        "/",
        [](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("audit up");
            cb(resp);
        },
        {Get}
    );
    app().registerHandler(
        "/health",
        [](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
            cb(HttpResponse::newHttpResponse());
        },
        {Get}
    );

    app().registerHandler(
        "/audit/services/refresh",
        [this](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
            service_.refreshOnce();
            auto ok = HttpResponse::newHttpResponse();
            ok->setContentTypeCode(CT_TEXT_PLAIN);
            ok->setBody("ok");
            cb(ok);
        },
        {Post}
    );

    app().registerHandler(
        "/audit/services/ping",
        [this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
            Json::Value j;
            if (!parseJson(req, j)) {
                auto bad = HttpResponse::newHttpResponse();
                bad->setStatusCode(k400BadRequest);
                bad->setContentTypeCode(CT_TEXT_PLAIN);
                bad->setBody("invalid json");
                return cb(bad);
            }
            const std::string name = j.get("service", "").asString();
            const std::string url  = j.get("base_url", "").asString();
            const std::string path = j.get("path", "").asString();
            if (name.empty() || url.empty() || path.empty()) {
                auto bad = HttpResponse::newHttpResponse();
                bad->setStatusCode(k400BadRequest);
                bad->setContentTypeCode(CT_TEXT_PLAIN);
                bad->setBody("missing fields: service, base_url, path");
                return cb(bad);
            }
            service_.pingSingleNow(name, url, path);
            cb(HttpResponse::newHttpResponse());
        },
        {Post}
    );

    app().registerHandler(
        "/audit/services",
        [this](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
            try {
                auto r = db_.client()->execSqlSync(
                    "SELECT service, instance, status, latency_ms, "
                    "       last_seen_ts AT TIME ZONE 'UTC' AS last_seen_utc "
                    "FROM service_status "
                    "ORDER BY service, instance;"
                );
                Json::Value arr(Json::arrayValue);
                for (const auto& row : r) {
                    Json::Value it;
                    it["service"]       = row["service"].as<std::string>();
                    it["instance"]      = row["instance"].as<std::string>();
                    it["status"]        = row["status"].as<std::string>();
                    it["latency_ms"]    = row["latency_ms"].as<int>();
                    it["last_seen_utc"] = row["last_seen_utc"].as<std::string>();
                    arr.append(std::move(it));
                }
                cb(HttpResponse::newHttpJsonResponse(arr));
            } catch (const std::exception& e) {
                auto err = HttpResponse::newHttpResponse();
                err->setStatusCode(k500InternalServerError);
                err->setContentTypeCode(CT_TEXT_PLAIN);
                err->setBody(std::string("db error: ") + e.what());
                cb(err);
            }
        },
        {Get}
    );
}
