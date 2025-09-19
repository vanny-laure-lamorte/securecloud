#pragma once
#include <drogon/drogon.h>

namespace audit {
void refreshByPing(std::function<void(const drogon::HttpResponsePtr&)>&& cb);

void schedulePeriodicRefresh(double seconds = 10.0);
}
