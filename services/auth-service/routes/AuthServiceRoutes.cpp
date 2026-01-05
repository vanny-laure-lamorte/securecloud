#include "AuthServiceRoutes.h"
#include "../utils/HashUtils.h"

AuthServiceRoutes::AuthServiceRoutes(DbConnection &db_)
    : userService_(db_) {}

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
            if (userService_.loginUser(email, password))
            {
                respJson["message"] = "Login successful";
                respJson["access_token"] = "temp-jwt-for-" + email; // TODO : Generate a real JWT token here
                // TODO : Add more user info / Create a model for user info
                // TODO : Set last login timestamp in DB
                // TODO : Add jwt in db to validate token later
                
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

            if (!json || !json->isMember("email") || !json->isMember("password") || !json->isMember("username"))
            {
                Json::Value respJson;
                respJson["status"] = "error";
                respJson["message"] = "Missing email, password, or username";

                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k400BadRequest);
                cb(resp);
                return;
            }

            std::string email = (*json)["email"].asString();
            std::string password = (*json)["password"].asString();
            std::string username = (*json)["username"].asString();
            std::string first_name = (*json)["first_name"].asString();
            std::string last_name = (*json)["last_name"].asString();

            try
            {
                userService_.registerUser(email, password, username, first_name, last_name);

                Json::Value respJson;
                respJson["status"] = "success";
                respJson["message"] = "User registered successfully";
                respJson["email"] = email;

                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k201Created);
                cb(resp);
            }
            catch (const std::runtime_error &e)
            {
                Json::Value respJson;
                respJson["status"] = "error";

                if (std::string(e.what()) == "USER_ALREADY_EXISTS")
                {
                    respJson["message"] = "User already exists with email " + email;

                    auto resp = HttpResponse::newHttpJsonResponse(respJson);
                    resp->setStatusCode(k409Conflict);
                    cb(resp);
                }
                else
                {
                    respJson["message"] = e.what();

                    auto resp = HttpResponse::newHttpJsonResponse(respJson);
                    resp->setStatusCode(k500InternalServerError);
                    cb(resp);
                }
            }
        },
        {Post});

    app().registerHandler(
        "/auth/logout",
        [this](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&cb)
        {
            auto json = req->getJsonObject();
            if (!json || !json->isMember("userId"))
            {
                Json::Value respJson;
                respJson["error"] = "Missing userId";

                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                resp->setStatusCode(k400BadRequest);
                cb(resp);
                return;
            }

            int userId = (*json)["userId"].asInt();

            Json::Value respJson;
            respJson["message"] = "Logout successful";

            auto resp = HttpResponse::newHttpJsonResponse(respJson);
            resp->setStatusCode(k200OK);
            cb(resp);
        },
        {Post});
}

// TODO : Implement logout route --> Update last active timestamp in DB