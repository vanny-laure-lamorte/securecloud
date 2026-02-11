#pragma once
#include <string>
#include <iostream>
#include <json/json.h>

namespace HttpUtils {

inline bool isSuccess(int code) { return code >= 200 && code < 300; }

inline const char* classOf(int code)
{
    if (code >= 200 && code < 300) return "2xx(SUCCESS)";
    if (code >= 300 && code < 400) return "3xx(REDIRECT)";
    if (code >= 400 && code < 500) return "4xx(CLIENT)";
    if (code >= 500 && code < 600) return "5xx(SERVER)";
    return "OTHER";
}

inline bool looksLikeJson(const std::string& body)
{
    for (char firstNonWhitespace : body)
    {
        if (firstNonWhitespace == ' ' ||
            firstNonWhitespace == '\n' ||
            firstNonWhitespace == '\r' ||
            firstNonWhitespace == '\t')
            continue;

        return firstNonWhitespace == '{' || firstNonWhitespace == '[';
    }
    return false;
}

inline std::string truncate(const std::string& s, size_t maxLen = 500)
{
    if (s.size() <= maxLen) return s;
    return s.substr(0, maxLen) + "...(truncated)";
}

inline std::string extractMessageFromBodySafe(const std::string &body)
{
    if (body.empty()) return {};
    if (!looksLikeJson(body)) return truncate(body);

    try
    {
        Json::Value json;
        Json::CharReaderBuilder b;
        std::string errs;
        auto r = std::unique_ptr<Json::CharReader>(b.newCharReader());

        if (!r->parse(body.data(), body.data() + body.size(), &json, &errs))
            return truncate(body);

        if (json.isObject())
        {
            if (json.isMember("message") && json["message"].isString()) return json["message"].asString();
            if (json.isMember("error")   && json["error"].isString())   return json["error"].asString();
            if (json.isMember("status")  && json["status"].isString())  return json["status"].asString();
        }

        return truncate(body);
    }
    catch (...)
    {
        return truncate(body);
    }
}

inline void logServiceCall(const std::string &serviceName,
                           const std::string &operation,
                           int code,
                           const std::string &body)
{
    std::cout << "[" << serviceName << "][" << operation << "] "
              << code << " " << classOf(code)
              << " | " << extractMessageFromBodySafe(body)
              << "\n";
}

}