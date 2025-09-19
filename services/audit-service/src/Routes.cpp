#include "Routes.h"
#include "AuditDb.h"
#include "Refresh.h"

#include <drogon/drogon.h>
#include <json/json.h>
#include <memory>
#include <string>

using namespace drogon;

namespace audit {

static bool parseJsonBody(const HttpRequestPtr& req, Json::Value& out) {
    Json::CharReaderBuilder b; std::string errs;
    const auto body = req->getBody();
    std::string s(body.data(), body.size());
    std::unique_ptr<Json::CharReader> r(b.newCharReader());
    return r->parse(s.data(), s.data()+s.size(), &out, &errs);
}

void registerRoutes() {
    app().registerHandler(
        "/audit/events",
        [](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
            Json::Value j;
            if (!parseJsonBody(req, j)) {
                auto bad = HttpResponse::newHttpResponse();
                bad->setStatusCode(k400BadRequest);
                bad->setContentTypeCode(CT_TEXT_PLAIN);
                bad->setBody("invalid json");
                return cb(bad);
            }
            const std::string ts   = j.get("ts", "").asString();
            const std::string svc  = j.get("service", "").asString();
            const std::string inst = j.get("instance", "default").asString();
            const std::string lvl  = j.get("level", "INFO").asString();
            const std::string cls  = j.get("class", "service").asString();
            const std::string typ  = j.get("type",  "event").asString();
            const std::string cid  = j.get("correlation_id", "").asString();

            Json::StreamWriterBuilder w; w["indentation"] = "";
            const std::string dataStr = Json::writeString(w, j.get("data", Json::objectValue));

            if (svc.empty()) {
                auto bad = HttpResponse::newHttpResponse();
                bad->setStatusCode(k400BadRequest);
                bad->setContentTypeCode(CT_TEXT_PLAIN);
                bad->setBody("missing field: service");
                return cb(bad);
            }

            try {
                if (!ts.empty()) {
                    gDb->execSqlSync(
                        "INSERT INTO events(ts,service,instance,level,class,type,correlation_id,data) "
                        "VALUES($1::timestamptz,$2,$3,$4,$5,$6,$7,$8::jsonb);",
                        ts, svc, inst, lvl, cls, typ, cid, dataStr
                    );
                } else {
                    gDb->execSqlSync(
                        "INSERT INTO events(ts,service,instance,level,class,type,correlation_id,data) "
                        "VALUES(NOW(),$1,$2,$3,$4,$5,$6,$7::jsonb);",
                        svc, inst, lvl, cls, typ, cid, dataStr
                    );
                }
                cb(HttpResponse::newHttpResponse());
            } catch (const std::exception& e) {
                auto err = HttpResponse::newHttpResponse();
                err->setStatusCode(k500InternalServerError);
                err->setContentTypeCode(CT_TEXT_PLAIN);
                err->setBody(std::string("db error: ") + e.what());
                cb(err);
            }
        },
        {Post}
    );

    app().registerHandler(
        "/audit/services",
        [](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
            if (req->getBody().empty()) {
                return refreshByPing(std::move(cb));
            }
            Json::Value j;
            if (!parseJsonBody(req, j)) {
                auto bad = HttpResponse::newHttpResponse();
                bad->setStatusCode(k400BadRequest);
                bad->setContentTypeCode(CT_TEXT_PLAIN);
                bad->setBody("invalid json");
                return cb(bad);
            }
            if (j.isObject() && j.get("action","").asString() == "refresh") {
                return refreshByPing(std::move(cb));
            }

            auto processOne = [](const Json::Value& it){
                const std::string svc  = it.get("service","").asString();
                const std::string inst = it.get("instance","default").asString();
                const std::string st   = it.get("status","").asString();
                const int latency      = it.get("latency_ms", 0).asInt();
                const std::string seen = it.get("seen_ts","").asString();
                if (svc.empty() || st.empty()) {
                    throw std::runtime_error("missing field: service or status");
                }
                upsertStatus(svc, inst, st, latency, seen);
            };

            try {
                if (j.isArray()) {
                    for (const auto& it : j) processOne(it);
                } else if (j.isObject()) {
                    processOne(j);
                } else {
                    auto bad = HttpResponse::newHttpResponse();
                    bad->setStatusCode(k400BadRequest);
                    bad->setContentTypeCode(CT_TEXT_PLAIN);
                    bad->setBody("expected object/array or {\"action\":\"refresh\"}");
                    return cb(bad);
                }
                cb(HttpResponse::newHttpResponse());
            } catch (const std::exception& e) {
                auto err = HttpResponse::newHttpResponse();
                err->setStatusCode(k400BadRequest);
                err->setContentTypeCode(CT_TEXT_PLAIN);
                err->setBody(std::string("bad request: ") + e.what());
                cb(err);
            }
        },
        {Post}
    );

    app().registerHandler(
        "/audit/services",
        [](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
            try {
                auto r = gDb->execSqlSync(
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

}
