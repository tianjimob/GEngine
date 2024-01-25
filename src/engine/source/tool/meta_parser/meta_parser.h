#pragma once

#include <filesystem>

class MetaParser {
public:
  MetaParser(const std::filesystem::path &metaHead, const std::filesystem::path& includePath);

private:
  std::filesystem::path m_metaHead;
  std::filesystem::path m_includePath;
};