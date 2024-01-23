#include "enum.h"

Enum::Enum(const Cursor &cursor) : m_name(cursor.getDisplayName()) {
  for (auto &child : cursor.getChildren()) {
    if (child.getKind() == CXCursor_EnumConstantDecl) {
      m_entries.emplace_back(child.getDisplayName());
    }
  }
}

const std::string& Enum::getEnumName() const
{
  return m_name;
}

const std::vector<std::string>& Enum::getConstantNamesList() const
{
  return m_entries;
}

