#include "util_string.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>

namespace GEngine {

namespace Util {
bool strToBool(const std::string &str) {
  std::string lstr = str;
  strToLower(lstr);

  if (lstr == "true" || str == "yes" || str == "on")
    return true;
  else if (lstr == "false" || lstr == "no" || lstr == "off")
    return false;
  else
    return std::strtol(lstr.c_str(), nullptr, 10);
}

void strToLower(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

} // namespace Util

} // namespace GEngine