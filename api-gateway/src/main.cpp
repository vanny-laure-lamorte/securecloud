
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

    outReq->setContentTypeCode(inReq->getContentType());

    for (const auto &h : inReq->getHeaders())
    {
        if (h.first == "host" || h.first == "Host")
            continue;
        outReq->addHeader(h.first, h.second);
    }

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
                                 out->setStatusCode(resp->getStatusCode());
                                 out->setContentTypeCode(CT_APPLICATION_JSON);
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

int getUserIdFromToken(const std::string &authHeader)
{
    (void)authHeader;
    return -1;
}

int main()
{
    const std::string AUTH_URL  = "http://auth-service-api:8081";
    const std::string MSG_URL   = "http://messaging-service-api:8082";
    const std::string AUDIT_URL = "http://audit-service:8083";

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
    "/auth/logout",
    [authClient](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&cb)
    {
        auto json = req->getJsonObject();
        std::string email;

        if (json && json->isMember("email"))
        {
            email = (*json)["email"].asString();
        }

        std::cout << "[AuthService] Logout requested";
        if (!email.empty())
            std::cout << " for email=" << email;
        std::cout << std::endl;

        Json::Value respJson;
        respJson["message"] = "Logout successful";

        auto resp = HttpResponse::newHttpJsonResponse(respJson);
        resp->setStatusCode(k200OK);
        cb(resp);
    },
    {Post});

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
    app().registerPreRoutingAdvice(
        [msgClient](const HttpRequestPtr &req,
                    AdviceCallback &&acb,
                    AdviceChainCallback &&accb)
        {
            const std::string path = req->path();

            if (path.rfind("/messaging/", 0) == 0 || path == "/messaging")
            {
                std::cout << "[Gateway] Client " << req->peerAddr().toIpPort()
                        << " -> " << req->path() << std::endl;

                forwardToBackend(req, msgClient, path, std::move(acb));
                return;
            }
            accb();
        });

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