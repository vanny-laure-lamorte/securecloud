#pragma once
#include <string>
#include <iostream>
#include <json/json.h>

namespace HttpUtils {

inline bool isSuccess(int code) {
    return code >= 200 && code < 300;
}

inline std::string extractMessageFromBody(const std::string &body) {
    if (body.empty())
        return {};

    Json::Value json;
    Json::CharReaderBuilder b;
    std::string errs;
    std::unique_ptr<Json::CharReader> r(b.newCharReader());

    if (!r->parse(body.data(), body.data() + body.size(), &json, &errs)) {
        return body;
    }

    if (json.isMember("message") && json["message"].isString())
        return json["message"].asString();
    if (json.isMember("error") && json["error"].isString())
        return json["error"].asString();
    if (json.isMember("status") && json["status"].isString())
        return json["status"].asString();

    return body;
}

inline void logServiceCall(const std::string &serviceName,
                           const std::string &operation,
                           int code,
                           const std::string &body)
{
    const char *cls = "OTHER";
    if (code >= 200 && code < 300)      cls = "2xx(SUCCESS)";
    else if (code >= 300 && code < 400) cls = "3xx(REDIRECT)";
    else if (code >= 400 && code < 500) cls = "4xx(CLIENT)";
    else if (code >= 500 && code < 600) cls = "5xx(SERVER)";

    std::string msg = extractMessageFromBody(body);

    std::cout << "[" << serviceName << "][" << operation << "] "
              << code << " " << cls
              << " | " << msg << "\n";
}

}
