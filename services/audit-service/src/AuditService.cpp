#include "AuditService.h"

AuditService::AuditService(AuditRepository& repo,
                           trantor::EventLoop* eventLoop)
    : repo_(repo), eventLoop_(eventLoop) {
    targets_ = {
        {"auth", "http://auth-service-api:8081", "/auth/ping"},
        {"messaging", "http://127.0.0.1:8082", "/messaging/ping"},
    };
}

void AuditService::startScheduler(double intervalSeconds) {
    drogon::app().getLoop()->runEvery(intervalSeconds, [this]() {
        this->refreshOnce();
    });
}

void AuditService::refreshOnce() {
    for (const auto& target : targets_) {
        pingAndRecord(target);
    }
}

std::optional<TargetService> AuditService::findTargetByName(const std::string& name) const {
    for (const auto& t : targets_) {
        if (t.serviceName == name) return t;
    }
    return std::nullopt;
}

void AuditService::pingSingleNow(const std::string& serviceName) {
    auto target = findTargetByName(serviceName);
    if (!target) {
        throw std::runtime_error("Unknown service: " + serviceName);
    }
    pingAndRecord(*target);
}
void AuditService::setTargets(std::vector<TargetService> targets) {
    targets_ = std::move(targets);
}

void AuditService::addTarget(TargetService target) {
    targets_.push_back(std::move(target));
}

void AuditService::pingAndRecord(const TargetService& t) {
    auto http = HttpClient::newHttpClient(t.baseUrl, eventLoop_);
    auto req  = HttpRequest::newHttpRequest();
    req->setMethod(Get);
    req->setPath(t.healthPath);

    auto start = std::chrono::steady_clock::now();
    auto prom  = std::make_shared<std::promise<std::tuple<bool,int,int>>>();
    auto fut   = prom->get_future();

    http->sendRequest(req, [prom, start](ReqResult r, const HttpResponsePtr& resp) {
        const int latency = (int)std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - start).count();
        const int code = resp ? (int)resp->getStatusCode() : -1;
        const bool up  = (r == ReqResult::Ok && resp && resp->getStatusCode() == 200);
        prom->set_value(std::make_tuple(up, code, latency));
    });

    std::thread([this, f = std::move(fut), name = t.serviceName, instance = t.baseUrl]() mutable {
        auto [up, code, latency] = f.get();
        (void)code;
        try {
            if (!repo_.statusExists(name, instance)) {
                if (up) repo_.insertStatusUp(name, instance, latency);
                else    repo_.insertStatusDown(name, instance);
            } else {
                if (up) repo_.updateStatusUp(name, instance, latency);
                else    repo_.updateStatusDown(name, instance);
            }
        } catch (const std::exception& e) {
            std::cerr << "[audit] repo error: " << e.what() << "\n";
        }
    }).detach();
}

std::vector<ServiceStatus> AuditService::getStatusesByService(const std::string& serviceName) const {
    return repo_.fetchStatusesByService(serviceName);
}