#include "field.h"
#include "util.h"
#include "clang-c/Index.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

Field::Field(const Cursor &cursor)
    : m_fieldName(cursor.getSpelling()),
      m_fieldTypeName(cursor.getType().GetDisplayName()),
      m_isEnum(cursor.getType().GetKind() == CXTypeKind::CXType_Enum) {
  if (isCArray(cursor)) {
    m_arrayType = ArrayType::CArray;
    m_cArraySize = cursor.getType().getArraySize();
    m_elemTypeName = cursor.getType().getElementType().GetDisplayName();
  } else if (isSizeArray(cursor)) {
    m_arrayType = ArrayType::SizeArray;
    m_elemTypeName =
        cursor.getType().getTemplateArgumentAsType(0).GetDisplayName();
  } else {
    m_arrayType = ArrayType::None;
  }
}

const std::string &Field::getFieldName() const { return m_fieldName; }

const std::string &Field::getFieldTypeName() const { return m_fieldTypeName; }

bool Field::isEnum() const { return m_isEnum; }

Field::ArrayType Field::getArrayType() const { return m_arrayType; }

size_t Field::getCArraySize() const { return m_cArraySize; }

const std::string &Field::getElementTypeName() const { return m_elemTypeName; }


bool Field::isCArray(const Cursor &cursor) const {
  // support random access operator[] and can't resize
  static const char *cArrayTypeNames[]{"std::array"};
  if (cursor.getType().isArray())
    return true;
  std::string typeName = cursor.getType().GetDisplayName();
  for (const char *name : cArrayTypeNames) {
    if (typeName.find(std::string{name}) != std::string::npos) {
      return true;
    }
  }
  return false;
}


bool Field::isSizeArray(const Cursor &cursor) const {
  // support random access operator[] and resize
  static const char *sizeTypeNames[] {
    "std::vector", "std::deque"
  };
  std::string typeName = cursor.getType().GetDisplayName();
  for (const char *name : sizeTypeNames) {
    if (typeName.find(std::string{name}) != std::string::npos) {
      return true;
    }
  }
  return false;
}
