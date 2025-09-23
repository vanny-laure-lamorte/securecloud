#pragma once
#include <drogon/drogon.h>

namespace audit {

// Refresh service statuses by pinging their health endpoints
void refreshByPing(std::function<void(const drogon::HttpResponsePtr&)>&& cb);

// Schedule periodic refresh of service statuses
void schedulePeriodicRefresh(double seconds = 10.0);
}
