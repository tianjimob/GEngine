#pragma once

#include "cursor.h"
#include "field.h"
#include "method.h"

#include <string>
#include <vector>

class Class {
public:
  Class(const Cursor& cursor);

  const std::string &getClassName() const;
  const std::vector<Field> &getFieldsList() const;
  const std::vector<Method> &getMethodsList() const;
  const std::string& getSuperClassName() const;
  
private:
  std::string m_className;
  std::vector<Field> m_fields;
  std::vector<Method> m_methods;
  std::string m_superClassName;
};