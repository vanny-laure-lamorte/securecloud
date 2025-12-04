#include "AuthServiceRoutes.h"
#include "../utils/HashUtils.h"

AuthServiceRoutes::AuthServiceRoutes(DbConnection &db_)
    : userRepository_(db_) {}

void AuthServiceRoutes::registerRoutes()
{
    using namespace drogon;

    app().registerHandler(
        "/auth/ping",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&cb)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("accessing login service");
            cb(resp);
        },
        {Get});

    app().registerHandler(
        "/auth/login",
        [this](const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&cb)
        {
            auto json = req->getJsonObject();
            if (!json || !json->isMember("email") || !json->isMember("password"))
            {
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
            if (email == "test@test.com" && password == "123456")
            {
                respJson["message"] = "Login successful";
                respJson["access_token"] = "temp-jwt-for-" + email;

                try
                {
                    std::string username = userRepository_.getUsernameByEmail(email);
                    respJson["username"] = username;
                }
                catch (const std::exception &e)
                {
                    respJson["username"] = "unknown";
                }

                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k200OK);
                cb(resp);
            }
            else
            {
                respJson["error"] = "Invalid email or password";
                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k401Unauthorized);
                cb(resp);
            }
        },
        {Post});

    app().registerHandler(
    "/auth/register",
    [this](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&cb)
    {
        auto json = req->getJsonObject();

        if (!json || !json->isMember("email") || !json->isMember("password") || !json->isMember("user_id"))
        {
            Json::Value respJson;
            respJson["status"] = "error";
            respJson["message"] = "Missing email, password, or user_id";

            auto resp = HttpResponse::newHttpJsonResponse(respJson);
            resp->setStatusCode(k400BadRequest);
            cb(resp);
            return;
        }

        std::string email = (*json)["email"].asString();
        std::string password = (*json)["password"].asString();
        int userId = (*json)["user_id"].asInt();

        std::string salt = HashUtils::generateSalt(10);
        std::string hashedPassword = HashUtils::hashPassword(password, salt);

        try
        {
            userRepository_.addUser(email, hashedPassword, userId);

            Json::Value respJson;
            respJson["status"] = "success";
            respJson["message"] = "User registered successfully";
            respJson["email"] = email;

            auto resp = HttpResponse::newHttpJsonResponse(respJson);
            resp->setStatusCode(k201Created);
            cb(resp);

        }
        catch (const std::exception &e)
        {
            std::cerr << "Error registering user: " << e.what() << std::endl;

            Json::Value respJson;
            respJson["status"] = "error";
            respJson["message"] = std::string("Registration failed: ") + e.what();

            auto resp = HttpResponse::newHttpJsonResponse(respJson);
            resp->setStatusCode(k500InternalServerError);
            cb(resp);
        }
    },
    {Post});
}
