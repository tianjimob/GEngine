#include "config_cache_ini.h"
#include "util_string.h"
#include "core/log/logger.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <string>
#include <utility>

namespace GEngine {

DECLARE_LOG_CATEGORY(LogConfigIniFile);

bool ConfigIniFile::parse(const std::string &filename) {
  std::ifstream ini{filename};
  if (!ini.is_open()) {
    LOG_ERROR(LogConfigIniFile, "Can not open file : {}", filename);
    return false;
  }

  ConfigSection *section;

  std::string line;
  while (std::getline(ini, line)) {
    std::string vline = line;

    size_t startPos = 0;
    size_t endPos = line.size();

    // strip leading spaces from the current line
    while (std::isspace(line[startPos]))
      ++startPos;

    // strip trailing spaces from the current line
    while (std::isspace(line[endPos - 1]))
      --endPos;

    if (vline[startPos] == '[' && vline[endPos - 1] == ']') {
      ++startPos;
      std::string sectionName =
          vline.substr(startPos, endPos - startPos - 1);
      auto it = m_sections.find(sectionName.data());
      if (it == m_sections.end()) {
        auto res = m_sections.emplace(sectionName.data(), ConfigSection{});
        section = &(res.first->second);
      } else
        section = &it->second;
    } else if (section && startPos < vline.size()) {
      // remove comment line
      if (vline[startPos] == ';')
        continue;

      // ignore any lines that don't contain a key-value pair
      auto equPos = vline.find('=');
      if (equPos == std::string::npos)
        continue;

      auto valuePos = equPos + 1;

      // strip trailing space from the key
      while (std::isspace(vline[equPos - 1]))
        --equPos;

      // strip leading space from the property value
      while (std::isspace(vline[valuePos]))
        ++valuePos;

      // strip trailing space from the property value
      while (std::isspace(vline[endPos - 1]))
        --endPos;

      std::string key = vline.substr(startPos, equPos - startPos);

      if (vline[valuePos] == '"' || vline[valuePos] == '(')
        ++valuePos;
      if (vline[endPos - 1] == '"' || vline[endPos - 1] == ')')
        --endPos;
      std::string value = vline.substr(valuePos, endPos - valuePos);

      (*section).emplace(key.data(), value.data());
    }
  }
  return true;
}

ConfigSection *ConfigIniFile::find(const std::string sectionName) {
  auto it = m_sections.find(sectionName);
  if (it != m_sections.end()) {
    return &(it->second);
  } else
    return nullptr;
}

ConfigCacheIni& ConfigCacheIni::instance()
{
  static ConfigCacheIni instance;
  return instance;
}

bool ConfigCacheIni::getString(const std::string &sectionName,
                               const std::string &key,
                               const std::string &filename,
                               std::string &value) {
  ConfigIniFile *file = find(filename);
  if (!file)
    return false;

  ConfigSection *section = file->find(sectionName);
  if (!section)
    return false;

  auto valueIt = section->find(key);
  if (valueIt != section->end()) {
    value = valueIt->second;
    return true;
  } else
    return false;
}

std::string ConfigCacheIni::getStr(const std::string &sectionName, const std::string &key,
                 const std::string &filename)
{
  std::string res;
  getString(sectionName, key, filename, res);
  return res;
}

bool ConfigCacheIni::getInt(const std::string &sectionName, const std::string &key,
              const std::string &filename, int32_t &value)
{
  std::string text;
  if (getString(sectionName, key, filename, text)) {
    value = std::strtol(text.c_str(), nullptr, 10);
    return true;
  }
  return false;
}

bool ConfigCacheIni::getFloat(const std::string &sectionName, const std::string &key,
              const std::string &filename, float &value)
{
  std::string text;
  if (getString(sectionName, key, filename, text)) {
    value = std::strtof(text.c_str(), nullptr);
    return true;
  }
  return false;
}

bool ConfigCacheIni::getDouble(const std::string &sectionName, const std::string &key,
              const std::string &filename, double &value)
{
  std::string text;
  if (getString(sectionName, key, filename, text)) {
    value = std::strtod(text.c_str(), nullptr);
    return true;
  }
  return false;
}

bool ConfigCacheIni::getBool(const std::string &sectionName, const std::string &key,
              const std::string &filename, bool &value)
{
  std::string text;
  if (getString(sectionName, key, filename, text)) {
    value = Util::strToBool(text);
    return true;
  }
  return false;
}

bool ConfigCacheIni::getStrings(const std::string &sectionName,
                                const std::string &key,
                                const std::string &filename,
                                std::vector<std::string> &value) {
  ConfigIniFile *file = find(filename);
  if (!file)
    return false;

  ConfigSection *section = file->find(sectionName);
  if (!section)
    return false;

  bool found = false;
  auto [begin, end] = section->equal_range(key);
  for (auto it = begin; it != end; ++it) {
    value.emplace_back(it->second);
    found = true;
  }
  return found;
}

ConfigIniFile *ConfigCacheIni::find(const std::string &filename) {
  auto it = m_configs.find(filename);
  if (it != m_configs.end()) {
    return &(it->second);
  } else {
    auto [it, _] = m_configs.emplace(filename, ConfigIniFile{});
    if (it->second.parse(filename))
      return &it->second;
    else
      return nullptr;
  }
}

} // namespace GEngine