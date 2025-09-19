#include "ServiceTargets.h"

namespace audit
{
    const std::vector<Target> &targets()
    {
        static const std::vector<Target> t = {
            {"auth", "http://127.0.0.1:8081", "/auth/ping"},
            {"messaging", "http://127.0.0.1:8082", "/messaging/ping"},
        };
        return t;
    }
}
