#include <drogon/drogon.h>

int main() {
    using namespace drogon;

    app().addListener("0.0.0.0", 8081);

    app().registerHandler(
        "/auth/ping",
        [](const HttpRequestPtr&,
           std::function<void (const HttpResponsePtr&)>&& cb) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody("accessing login service");
            cb(resp);
        },
        {drogon::Get}
    );

    app().run();
    return 0;
}
