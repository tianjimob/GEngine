#pragma once

#include <string>
#include <vector>
namespace GEngine {

namespace Util {

bool strToBool(const std::string &str);

void strToLower(std::string &str);

std::vector<std::string> split(const std::string &str, char delimiter);

}

}