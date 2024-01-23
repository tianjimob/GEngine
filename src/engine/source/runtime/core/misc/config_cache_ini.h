#pragma once

#include <map>
#include <string>

namespace GEngine {

using ConfigSection = std::map<std::string, std::string>;

class ConfigIniFile {
public:
  bool parse(const std::string &filename);

  ConfigSection *find(const std::string sectionName);

private:
  
  std::map<std::string, ConfigSection> m_sections;
};

class ConfigCacheIni {
public:
  static ConfigCacheIni &instance();
  
  bool getString(const std::string &sectionName, const std::string &key,
                 const std::string &filename, std::string &value);

  std::string getStr(const std::string &sectionName, const std::string &key,
                     const std::string &filename);
  bool getInt(const std::string &sectionName, const std::string &key,
              const std::string &filename, int32_t &value);
  bool getFloat(const std::string &sectionName, const std::string &key,
              const std::string &filename, float &value);
  bool getDouble(const std::string &sectionName, const std::string &key,
              const std::string &filename, double &value);
  bool getBool(const std::string &sectionName, const std::string &key,
              const std::string &filename, bool &value);

private:
  std::map<std::string, ConfigIniFile> m_configs;

 ConfigIniFile * find(const std::string& filename);
};

}