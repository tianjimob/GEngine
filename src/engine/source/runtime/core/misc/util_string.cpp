#include "util_string.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

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

std::vector<std::string> split(const std::string &str, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(str);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.emplace_back(token);
  }
  return tokens;
}

void removeWhiteSpace(std::string &str) {
  str.erase(std::remove_if(str.begin(), str.end(),
                           [](unsigned ch) { return std::isspace(ch); }),
            str.end());
}

std::string addQuotesAndCheck(const std::string &str) {
  if (str.find('"') != std::string::npos)
    return {};
  else
    return '"' + str + '"';
}
} // namespace Util

} // namespace GEngine