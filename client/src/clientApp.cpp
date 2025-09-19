#include "ClientApp.h"
#include <json/json.h>

#include <iostream>
#include <future>
#include <memory>

using namespace drogon;

ClientApp::ClientApp(const HttpClientPtr &client) : client_(client) {}

void ClientApp::printMenu()
{
    std::cout << "Menu:\n"
                 "  1) Ping auth via API Gateway (/auth/ping)\n"
                 "  2) Ping messaging via API Gateway (/messaging/ping)\n"
                 "  3) POST audit event via API Gateway (/audit/events)\n"
                 "  4) GET services status (DB) via API Gateway (/audit/services)\n"
                 "  5) POST services status via API Gateway (/audit/services)\n"
                 "  0) Quit\n> ";
}

std::pair<int, std::string>
ClientApp::sendAndWait(const HttpRequestPtr &req, std::chrono::seconds timeout)
{
    auto prom = std::make_shared<std::promise<std::pair<int, std::string>>>();
    auto fut = prom->get_future();

    client_->sendRequest(req, [prom](ReqResult r, const HttpResponsePtr &resp)
                         {
        if (r == ReqResult::Ok && resp) {
            auto v = resp->getBody();
            prom->set_value({(int)resp->getStatusCode(), std::string(v.data(), v.size())});
        } else {
            prom->set_value({502, "bad gateway (no response)"});
        } });

    if (fut.wait_for(timeout) != std::future_status::ready)
    {
        return {408, "timeout"};
    }
    return fut.get();
}

void ClientApp::ping(const std::string &path)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath(path);

    auto [code, body] = sendAndWait(req);
    std::cout << "[Client] " << code << " | " << body << "\n";
}

void ClientApp::postAuditEvent()
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Post);
    req->setPath("/audit/events");
    req->addHeader("Content-Type", "application/json");
    std::string payload =
        R"({"service":"client","instance":"cli-1","level":"INFO","class":"service","type":"client.test","data":{}})";
    req->setBody(std::move(payload));

    auto [code, body] = sendAndWait(req);
    std::cout << "[Client] " << code << " | " << (body.empty() ? "(empty)" : body) << "\n";
}

void ClientApp::getServicesStatus()
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/audit/services");

    auto [code, body] = sendAndWait(req);
    if (code != 200)
    {
        std::cout << "[Client] " << code << " | " << body << "\n";
        return;
    }

    Json::Value arr;
    Json::CharReaderBuilder b;
    std::string errs;
    std::unique_ptr<Json::CharReader> r(b.newCharReader());
    if (!r->parse(body.data(), body.data() + body.size(), &arr, &errs) || !arr.isArray())
    {
        std::cout << "[Client] invalid JSON\n";
        return;
    }

    for (const auto &it : arr)
    {
        const std::string svc = it.get("service", "").asString();
        const std::string inst = it.get("instance", "").asString();
        const std::string stat = it.get("status", "").asString();
        const int lat = it.get("latency_ms", 0).asInt();
        const std::string ls = it.get("last_seen_utc", "").asString();
        std::cout << svc << " - " << stat << " - " << lat << "ms - " << inst << " Last seen - " << ls << "\n";
    }
}

void ClientApp::postServicesStatus()
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Post);
    req->setPath("/audit/services");

    auto [code, body] = sendAndWait(req);
    if (code != 200)
    {
        std::cout << "[Client] " << code << " | " << body << "\n";
        return;
    }
    else
    {
        std::cout << "[Audit Service] " << code << " | Services status refreshed\n";
    }
}

void ClientApp::run()
{
    while (running)
    {
        printMenu();
        int choice = -1;
        if (!(std::cin >> choice))
        {
            std::cout << "[Client] Bye.\n";
            break;
        }

        switch (choice)
        {
        case 0:
            std::cout << "[Client] Bye.\n";
            return;
        case 1:
            ping("/auth/ping");
            break;
        case 2:
            ping("/messaging/ping");
            break;
        case 3:
            postAuditEvent();
            break;
        case 4:
            getServicesStatus();
            break;
        case 5:
            postServicesStatus();
            break;
        default:
            std::cout << "[Client] Invalid choice.\n";
            break;
        }
    }
}
