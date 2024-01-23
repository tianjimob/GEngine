#include "field.h"
#include <iostream>

Field::Field(const Cursor &cursor)
    : m_fieldName(cursor.getSpelling()),
      m_fieldTypeName(cursor.getType().GetDisplayName()),
      m_isEnum(cursor.getType().GetKind() == CXTypeKind::CXType_Enum) {
  if (cursor.getType().isArray()) {
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

bool Field::isSizeArray(const Cursor &cursor) const {
  std::string typeName = cursor.getType().GetDisplayName();
  if (typeName.find("std::vector") != std::string::npos) {
    return true;
  } else {
    return false;
  }
}
