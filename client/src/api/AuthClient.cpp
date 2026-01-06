#include "api/AuthClient.h"
#include "core/HttpUtils.h"
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

    HttpUtils::logServiceCall("Auth", "login", code, body);

    if (!HttpUtils::isSuccess(code))
        return false;

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

    state_->authenticated = true;
    state_->email = email;
    state_->jwt = token;

    std::cout << "[Auth] Logged in, JWT stored\n";
    return true;
}

bool AuthClient::logout(const std::string &email)
{
    Json::Value payload;
    payload["email"] = email;

    auto req = HttpRequest::newHttpJsonRequest(payload);
    req->setMethod(Post);
    req->setPath("/auth/logout");

    auto [code, body] = http_.send(req);
    HttpUtils::logServiceCall("Auth", "logout", code, body);

    if (!HttpUtils::isSuccess(code))
    {
        std::cout << "[Auth] Logout failed.\n";
        return false;
    }

    state_->authenticated = false;
    state_->userName = "";
    state_->email = "";
    state_->jwt = "";

    std::cout << "[Auth] Logout successful.\n";
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

bool AuthClient::registerUser(const std::string &email,
                              const std::string &password,
                              const std::string &username,
                              const std::string &firstName,
                              const std::string &lastName)
{
    Json::Value payload;
    payload["email"] = email;
    payload["password"] = password;
    payload["username"] = username;
    payload["first_name"] = firstName;
    payload["last_name"] = lastName;

    auto req = HttpRequest::newHttpJsonRequest(payload);
    req->setMethod(Post);
    req->setPath("/auth/register");

    auto [code, body] = http_.send(req);

    HttpUtils::logServiceCall("Auth", "register", code, body);

    return HttpUtils::isSuccess(code);
}