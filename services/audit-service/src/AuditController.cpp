#include "AuditController.h"

AuditController::AuditController(DbConnection& db, trantor::EventLoop* loop)
    : repo_(db), service_(repo_, loop) {
    repo_.ensureSchema();
    service_.startScheduler(10.0);
}

bool AuditController::parseJson(const HttpRequestPtr& req, Json::Value& out) {
    Json::CharReaderBuilder b;
    std::string errs;
    const auto buf = req->getBody();
    std::string s(buf.data(), buf.size());
    auto r = std::unique_ptr<Json::CharReader>(b.newCharReader());
    return r->parse(s.data(), s.data() + s.size(), &out, &errs);
}

HttpResponsePtr AuditController::handlePing(const HttpRequestPtr& req) {
    Json::Value j;
    if (!parseJson(req, j)) {
        auto bad = HttpResponse::newHttpResponse();
        bad->setStatusCode(k400BadRequest);
        bad->setContentTypeCode(CT_TEXT_PLAIN);
        bad->setBody("invalid json");
        return bad;
    }
    const std::string name = j.get("service", "").asString();
    const std::string url  = j.get("base_url", "").asString();
    const std::string path = j.get("path", "").asString();
    if (name.empty() || url.empty() || path.empty()) {
        auto bad = HttpResponse::newHttpResponse();
        bad->setStatusCode(k400BadRequest);
        bad->setContentTypeCode(CT_TEXT_PLAIN);
        bad->setBody("missing fields: service, base_url, path");
        return bad;
    }

    try {
        service_.pingSingleNow(name, url, path);
        return HttpResponse::newHttpResponse();
    } catch (const std::exception& e) {
        auto err = HttpResponse::newHttpResponse();
        err->setStatusCode(k500InternalServerError);
        err->setBody(std::string("error: ") + e.what());
        return err;
    }
}

HttpResponsePtr AuditController::handleList() {
    try {
        auto statuses = repo_.fetchAllStatuses();
        Json::Value arr(Json::arrayValue);
        for (const auto& s : statuses) {
            Json::Value it;
            it["service"]       = s.service;
            it["instance"]      = s.instance;
            it["status"]        = s.status;
            it["latency_ms"]    = s.latencyMs;
            it["last_seen_utc"] = s.lastSeenUtc;
            arr.append(it);
        }
        return HttpResponse::newHttpJsonResponse(arr);
    } catch (const std::exception& e) {
        auto err = HttpResponse::newHttpResponse();
        err->setStatusCode(k500InternalServerError);
        err->setBody(std::string("db error: ") + e.what());
        return err;
    }
}

HttpResponsePtr AuditController::handleRefresh() {
    auto resp = HttpResponse::newHttpResponse();
    try {
        service_.refreshOnce();
        resp->setContentTypeCode(CT_TEXT_PLAIN);
        resp->setBody("ok");
        return resp;
    } catch (const std::exception& e) {
        resp->setStatusCode(k500InternalServerError);
        resp->setBody(std::string("error: ") + e.what());
        return resp;
    }
}
