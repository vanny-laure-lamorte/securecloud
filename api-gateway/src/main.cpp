
#define _CRT_SECURE_NO_WARNINGS
#include <drogon/drogon.h>
#include <json/json.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>

using namespace drogon;

static void forwardToBackend(const HttpRequestPtr& inReq,
                             const HttpClientPtr& backend,
                             const std::string& backendPath,
                             std::function<void (const HttpResponsePtr&)>&& cb) {
    auto outReq = HttpRequest::newHttpRequest();
    outReq->setMethod(inReq->getMethod());
    outReq->setPath(backendPath);

    // copy body if present (string_view -> string)
    auto inBody = inReq->getBody();
    if (!inBody.empty()) {
        std::string body(inBody.data(), inBody.size());
        outReq->setBody(std::move(body));
    }

    backend->sendRequest(outReq,
        [cb = std::move(cb)](ReqResult r, const HttpResponsePtr& resp) mutable {
            if (r == ReqResult::Ok && resp) {
                auto view = resp->getBody();
                std::string body(view.data(), view.size());
                std::cout << "[Gateway] Backend response: " << body << std::endl;

                auto out = HttpResponse::newHttpResponse();
                out->setContentTypeCode(CT_TEXT_PLAIN);
                out->setBody(std::move(body));
                cb(out);
            } else {
                std::cout << "[Gateway] Backend error (bad gateway)\n";
                auto out = HttpResponse::newHttpResponse();
                out->setStatusCode(k502BadGateway);
                out->setContentTypeCode(CT_TEXT_PLAIN);
                out->setBody("bad gateway");
                cb(out);
            }
        }
    );
}

// Targets for /admin/services (extend as you add services)
struct Target { std::string name, baseUrl, path; };
static const std::vector<Target> kTargets = {
    {"auth",      "http://127.0.0.1:8081", "/auth/ping"},
    {"messaging", "http://127.0.0.1:8082", "/messaging/ping"},
};

int main() {
    const std::string AUTH_URL = "http://127.0.0.1:8081";
    const std::string MSG_URL  = "http://127.0.0.1:8082";

    auto authClient = HttpClient::newHttpClient(AUTH_URL);
    auto msgClient  = HttpClient::newHttpClient(MSG_URL);

    app().addListener("0.0.0.0", 8080);

    // ===== GATEWAY ROOT =====
    app().registerHandler(
        "/",
        [](const HttpRequestPtr&,
           std::function<void (const HttpResponsePtr&)>&& cb) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("Gateway up");
            cb(resp);
        },
        {drogon::Get}
    );

    // ===== AUTH =====
    app().registerHandler(
        "/auth/{_path}",
        [authClient](const HttpRequestPtr& req,
                     std::function<void (const HttpResponsePtr&)>&& cb) {
            std::cout << "[Gateway] Client " << req->peerAddr().toIpPort()
                      << " -> " << req->path() << std::endl;
            forwardToBackend(req, authClient, req->path(), std::move(cb));
        },
        {drogon::Get, drogon::Post}
    );

    // ===== MESSAGING =====
    app().registerHandler(
        "/messaging/{_path}",
        [msgClient](const HttpRequestPtr& req,
                    std::function<void (const HttpResponsePtr&)>&& cb) {
            std::cout << "[Gateway] Client " << req->peerAddr().toIpPort()
                      << " -> " << req->path() << std::endl;
            forwardToBackend(req, msgClient, req->path(), std::move(cb));
        },
        {drogon::Get, drogon::Post}
    );

    // ===== SERVICE STATUS (Option A) =====
    app().registerHandler(
        "/admin/services",
        [](const HttpRequestPtr&,
           std::function<void (const HttpResponsePtr&)>&& cb) {
            auto results   = std::make_shared<Json::Value>(Json::arrayValue);
            auto remaining = std::make_shared<std::atomic<int>>( (int)kTargets.size() );
            auto mutex     = std::make_shared<std::mutex>();

            for (const auto& t : kTargets) {
                auto client = HttpClient::newHttpClient(t.baseUrl);
                auto req    = HttpRequest::newHttpRequest();
                req->setMethod(Get);
                req->setPath(t.path);
                auto start  = std::make_shared<std::chrono::steady_clock::time_point>(
                                  std::chrono::steady_clock::now());

                client->sendRequest(req,
                    [t, results, remaining, mutex, cb, start](ReqResult r, const HttpResponsePtr& resp) {
                        int latency = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                          std::chrono::steady_clock::now() - *start).count();
                        Json::Value item;
                        item["name"]       = t.name;
                        item["url"]        = t.baseUrl;
                        item["path"]       = t.path;
                        item["latency_ms"] = latency;
                        bool up = (r == ReqResult::Ok && resp && resp->getStatusCode() == 200);
                        item["status"]     = up ? "UP" : "DOWN";

                        {
                            std::lock_guard<std::mutex> lk(*mutex);
                            results->append(std::move(item));
                        }
                        if (remaining->fetch_sub(1) == 1) {
                            cb(HttpResponse::newHttpJsonResponse(*results));
                        }
                    }
                );
            }
        },
        {drogon::Get}
    );

    std::cout << "[Gateway] Listening on http://127.0.0.1:8080\n";
    app().run();
    return 0;
}
