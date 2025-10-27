#pragma once
#include "AuditRepository.h"
#include <drogon/drogon.h>
// #include <trantor/utils/EventLoop.h>
#include <string>
#include <vector>

struct TargetService {
    std::string serviceName;
    std::string baseUrl;
    std::string healthPath;
};

class AuditService {
public:
    AuditService(AuditRepository& repo,
                 trantor::EventLoop* eventLoop);

    void startScheduler(double intervalSeconds);
    void refreshOnce();
    void pingSingleNow(const std::string& serviceName,
                       const std::string& baseUrl,
                       const std::string& healthPath);

    void setTargets(std::vector<TargetService> targets);
    void addTarget(TargetService target);

private:
    void pingAndRecord(const TargetService& t);

    AuditRepository& repo_;
    std::vector<TargetService> targets_;
    trantor::EventLoop* eventLoop_;
};
