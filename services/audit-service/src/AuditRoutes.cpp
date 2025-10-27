#include "AuditRoutes.h"

AuditRoutes::AuditRoutes(DbConnection &db, trantor::EventLoop *loop)
    : controller_(db, loop) {}

void AuditRoutes::registerAll()
{

    app().registerHandler(
        "/audit/service_ping",
        [this](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&cb)
        {
            cb(controller_.handleServicePing(req));
        },
        {Post});

    app().registerHandler(
        "/audit/services",
        [this](const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&cb)
        {
            cb(controller_.handleList());
        },
        {Get});

    app().registerHandler(
        "/audit/services",
        [this](const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&cb)
        {
            cb(controller_.handleRefresh());
        },
        {Post});
    app().registerHandler(
        "/audit/get_one_service_status",
        [this](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&cb)
        {
            cb(controller_.getOneServiceStatus(req));
        },
        {Get});
}
