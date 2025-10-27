#pragma once

#include <common/db/DbConnection.h>
#include "AuditController.h"

class AuditRoutes {
public:
    AuditRoutes(DbConnection& db, trantor::EventLoop* loop);

    void registerAll();

private:
    AuditController controller_;
};
