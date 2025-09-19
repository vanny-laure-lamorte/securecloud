#pragma once
#include <string>
#include <vector>

namespace audit {
struct Target { std::string name, baseUrl, path; };
const std::vector<Target>& targets();
}
