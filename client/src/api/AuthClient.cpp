#include "api/AuthClient.h"
#include <iostream>

using namespace drogon;

bool AuthClient::login(const std::string &email, const std::string &password)
{
    Json::Value payload;
    payload["email"] = email;
    payload["password"] = password;

    auto req = HttpRequest::newHttpJsonRequest(payload);
    req->setMethod(Post);
    req->setPath("/auth/login");

    auto [code, body] = http_.send(req);
    if (code != 200)
    {
        std::cout << "[Auth] Login failed: " << code << " | " << body << "\n";
        return false;
    }

    Json::Value json;
    Json::CharReaderBuilder b;
    std::string errs;
    std::unique_ptr<Json::CharReader> r(b.newCharReader());
    if (!r->parse(body.data(), body.data() + body.size(), &json, &errs))
    {
        std::cout << "[Auth] Invalid JSON login response\n";
        return false;
    }

    const std::string token = json.get("access_token", "").asString();
    if (token.empty())
    {
        std::cout << "[Auth] No access_token in response\n";
        return false;
    }
    const std::string username = json.get("username", "").asString();
    state_->authenticated = true;
    state_->userNamme = username;
    state_->email = email;
    state_->jwt = token;
    std::cout << "[Auth] Logged in, JWT stored with : " << token << "\n";
    return true;
}

void AuthClient::ping()
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath("/auth/ping");

    auto [code, body] = http_.send(req);
    std::cout << "[Auth] " << code << " | " << body << "\n";
}

std::string AuthClient::getJwt() const
{
    return state_->jwt;
}