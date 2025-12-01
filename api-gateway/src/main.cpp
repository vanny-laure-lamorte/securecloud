
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

static void forwardToBackend(const HttpRequestPtr &inReq,
                             const HttpClientPtr &backend,
                             const std::string &backendPath,
                             std::function<void(const HttpResponsePtr &)> &&cb)
{
    auto outReq = HttpRequest::newHttpRequest();
    outReq->setMethod(inReq->getMethod());
    outReq->setPath(backendPath);

    auto inBody = inReq->getBody();
    if (!inBody.empty())
    {
        std::string body(inBody.data(), inBody.size());
        outReq->setBody(std::move(body));
    }

    backend->sendRequest(outReq,
                         [cb = std::move(cb)](ReqResult r, const HttpResponsePtr &resp) mutable
                         {
                             if (r == ReqResult::Ok && resp)
                             {
                                 auto view = resp->getBody();
                                 std::string body(view.data(), view.size());
                                 std::cout << "[Gateway] Backend response: " << body << std::endl;

                                 auto out = HttpResponse::newHttpResponse();
                                 out->setContentTypeCode(CT_TEXT_PLAIN);
                                 out->setBody(std::move(body));
                                 cb(out);
                             }
                             else
                             {
                                 std::cout << "[Gateway] Backend error (bad gateway)\n";
                                 auto out = HttpResponse::newHttpResponse();
                                 out->setStatusCode(k502BadGateway);
                                 out->setContentTypeCode(CT_TEXT_PLAIN);
                                 out->setBody("bad gateway");
                                 cb(out);
                             }
                         });
}

int main()
{
    const std::string AUTH_URL = "http://127.0.0.1:8081";
    const std::string MSG_URL = "http://127.0.0.1:8082";
    const std::string AUDIT_URL = "http://127.0.0.1:8083";

    auto authClient = HttpClient::newHttpClient(AUTH_URL);
    auto msgClient = HttpClient::newHttpClient(MSG_URL);
    auto auditClient = HttpClient::newHttpClient(AUDIT_URL);

    app().addListener("0.0.0.0", 8080);

    // ===== GATEWAY ROOT =====
    app().registerHandler(
        "/",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&cb)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("Gateway up");
            cb(resp);
        },
        {drogon::Get});

    // ===== AUTH SERVICE=====
    app().registerHandler(
        "/auth/{_path}",
        [authClient](const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&cb)
        {
            std::cout << "[Gateway] Client " << req->peerAddr().toIpPort()
                      << " -> " << req->path() << std::endl;
            forwardToBackend(req, authClient, req->path(), std::move(cb));
        },
        {drogon::Get, drogon::Post});

    // ===== MESSAGING SERVICE =====
    app().registerHandler(
        "/messaging/{_path}",
        [msgClient](const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&cb)
        {
            std::cout << "[Gateway] Client " << req->peerAddr().toIpPort()
                      << " -> " << req->path() << std::endl;
            forwardToBackend(req, msgClient, req->path(), std::move(cb));
        },
        {drogon::Get, drogon::Post});

    // ===== AUDIT SERVICE =====

    app().registerHandler(
        "/audit/{_path}",
        [auditClient](const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&cb)
        {
            std::cout << "[Gateway] Client " << req->peerAddr().toIpPort()
                      << " -> " << req->path() << std::endl;
            forwardToBackend(req, auditClient, req->path(), std::move(cb));
        },
        {drogon::Get, drogon::Post});

    std::cout << "[Gateway] Listening on http://127.0.0.1:8080\n";
    app().run();
    return 0;
}
