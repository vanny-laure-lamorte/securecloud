#include "Refresh.h"
#include "ServiceTargets.h"
#include "AuditDb.h"

#include <json/json.h>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>

using namespace drogon;

namespace audit {

void refreshByPing(std::function<void(const HttpResponsePtr&)>&& cb) {
    auto& tgts = targets();
    auto results   = std::make_shared<Json::Value>(Json::arrayValue);
    auto remaining = std::make_shared<std::atomic<int>>((int)tgts.size());
    auto mtx       = std::make_shared<std::mutex>();

    if (tgts.empty()) {
        cb(HttpResponse::newHttpJsonResponse(*results));
        return;
    }

    for (const auto& t : tgts) {
        auto cli  = HttpClient::newHttpClient(t.baseUrl);
        auto req  = HttpRequest::newHttpRequest();
        req->setMethod(Get);
        req->setPath(t.path);

        auto start = std::make_shared<std::chrono::steady_clock::time_point>(
            std::chrono::steady_clock::now());

        cli->sendRequest(req,
            [t, start, results, remaining, mtx, cb](ReqResult r, const HttpResponsePtr& resp) {
                int latency = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                  std::chrono::steady_clock::now() - *start).count();
                bool up = (r == ReqResult::Ok && resp && resp->getStatusCode() == 200);
                try {
                    upsertStatus(t.name, t.baseUrl, up ? "UP" : "DOWN", latency, "");
                } catch (const std::exception& e) {
                    std::cerr << "[audit] upsert failed: " << e.what() << "\n";
                }

                Json::Value it;
                it["service"]    = t.name;
                it["instance"]   = t.baseUrl;
                it["status"]     = up ? "UP" : "DOWN";
                it["latency_ms"] = latency;

                { std::lock_guard<std::mutex> lk(*mtx); results->append(std::move(it)); }
                if (remaining->fetch_sub(1) == 1) {
                    cb(HttpResponse::newHttpJsonResponse(*results));
                }
            }
        );
    }
}

void schedulePeriodicRefresh(double seconds) {
    drogon::app().getLoop()->runEvery(seconds, []() {
        refreshByPing([](const HttpResponsePtr&) {
        });
    });
}

}
