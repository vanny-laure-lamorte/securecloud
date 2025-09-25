#include "../include/AuthServiceRoutes.h"

void AuthServiceRoutes::registerRoutes() {
    using namespace drogon;

    app().registerHandler(
        "/auth/ping",
        [](const HttpRequestPtr&,
           std::function<void(const HttpResponsePtr&)>&& cb) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("accessing login service");
            cb(resp);
        },
        {Get}
    );

    app().registerHandler(
        "/auth/login",
        [](const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& cb) {
            auto json = req->getJsonObject();
            if (!json || !json->isMember("email") || !json->isMember("password")) {
                Json::Value respJson;
                respJson["error"] = "Missing email or password";

                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k400BadRequest);
                cb(resp);
                return;
            }

            std::string email = (*json)["email"].asString();
            std::string password = (*json)["password"].asString();

            Json::Value respJson;
            if (email == "test@test.com" && password == "123456") {
                respJson["message"] = "Login successful";
                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k200OK);
                cb(resp);
            } else {
                respJson["error"] = "Invalid email or password";
                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k401Unauthorized);
                cb(resp);
            }
        },
        {Post}
    );
}
