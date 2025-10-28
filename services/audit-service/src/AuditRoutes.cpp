#include "AuditRoutes.h"

AuditRoutes::AuditRoutes(DbConnection& db, trantor::EventLoop* loop)
    : controller_(db, loop) {}

void AuditRoutes::registerAll() {
    app().registerHandler(
        "/audit/services/ping",
        [this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
            cb(controller_.handlePing(req));
        },
        {Post}
    );

    app().registerHandler(
        "/audit/services",
        [this](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
            cb(controller_.handleList());
        },
        {Get}
    );

    app().registerHandler(
        "/audit/services",
        [this](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& cb) {
            cb(controller_.handleRefresh());
        },
        {Post}
    );
}
