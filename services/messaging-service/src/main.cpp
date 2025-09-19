#include <drogon/drogon.h>

int main() {
    using namespace drogon;

    app().addListener("0.0.0.0", 8082);

    app().registerHandler(
        "/messaging/ping",
        [](const HttpRequestPtr&,
           std::function<void (const HttpResponsePtr&)>&& cb) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("accessing messaging service");
            cb(resp);
        },
        {drogon::Get}
    );

    app().run();
    return 0;
}