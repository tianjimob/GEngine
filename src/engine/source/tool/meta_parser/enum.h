#pragma once

#include "cursor.h"
#include "namespace.h"
#include <string>
#include <vector>

class Enum {
public:
  Enum(const Cursor &cursor);

  const std::string &getEnumName() const;
  const std::vector<std::string>& getConstantNamesList() const;

private:
  std::string m_name;
  std::vector<std::string> m_entries;
  Namespace m_namespace;
};