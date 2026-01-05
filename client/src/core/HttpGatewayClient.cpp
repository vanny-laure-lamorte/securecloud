#include "core/HttpGatewayClient.h"
#include <future>

using namespace drogon;

std::pair<int, std::string>
HttpGatewayClient::send(const HttpRequestPtr &req, std::chrono::seconds timeout)
{
    std::cout << "[HttpGatewayClient] Sending request to " << req->getPath() << "\n";
    std::cout << "jwt : " << (state_ ? state_->jwt : "<no state>") << "\n";
    if (state_ && !state_->jwt.empty())
        req->addHeader("Authorization", "Bearer " + state_->jwt);

    auto prom = std::make_shared<std::promise<std::pair<int, std::string>>>();
    auto fut = prom->get_future();

    client_->sendRequest(req, [prom](ReqResult r, const HttpResponsePtr &resp)
                         {
        if ((int)resp->getStatusCode() >= 200 && (int)resp->getStatusCode() < 300)
        {
            auto v = resp->getBody();
            prom->set_value({(int)resp->getStatusCode(), std::string(v.data(), v.size())});
        }
        else if ((int)resp->getStatusCode() != 502)
        {
            auto v = resp->getBody();
            prom->set_value({(int)resp->getStatusCode(), std::string(v.data(), v.size())});
        }
        else
        {
            prom->set_value({502, "bad gateway (no response)"});
        } });

    if (fut.wait_for(timeout) != std::future_status::ready)
        return {408, "timeout"};

    return fut.get();
}
