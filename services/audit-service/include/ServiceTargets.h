#pragma once
#include <string>
#include <vector>

namespace audit {
struct Target { std::string name, baseUrl, path; };
// List of service targets to monitor
const std::vector<Target>& targets();
}
