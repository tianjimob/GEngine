#pragma once

#include "cursor.h"
#include "namespace.h"
#include <string>

class Field {
public:
  enum class ArrayType {
    None,
    CArray,
    SizeArray
  };
  
  Field(const Cursor &cursor);

  const std::string& getFieldName() const;
  const std::string &getFieldTypeName() const;
  bool isEnum() const;
  ArrayType getArrayType() const;
  size_t getCArraySize() const;
  const std::string& getElementTypeName() const;

private:
  std::string m_fieldName;
  std::string m_fieldTypeName;
  bool m_isEnum;
  ArrayType m_arrayType;

  size_t m_cArraySize;
  std::string m_elemTypeName;

private:
  bool isCArray(const Cursor& cursor) const;
  bool isSizeArray(const Cursor& cursor) const;

};