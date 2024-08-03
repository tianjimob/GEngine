#include "class.h"
#include "clang-c/Index.h"
#include <cassert>
#include <cstdlib>
#include <iostream>


Class::Class(const Cursor& cursor):m_className(cursor.getType().GetDisplayName()),m_isAbstract(false)
{
  for (auto &child : cursor.getChildren()) {
    switch (child.getKind()) {
    case CXCursor_CXXBaseSpecifier:
      if(m_superClassName.empty())
        m_superClassName = child.getType().GetDisplayName();
      else {
        std::cerr << "meta parser cannot support multiple inheritance"
                  << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case CXCursor_FieldDecl:
      if(child.hasAnnotateAttr("meta-field"))
        m_fields.emplace_back(child);
      break;
    case CXCursor_CXXMethod:
      if(child.hasAnnotateAttr("meta-method"))
        m_methods.emplace_back(child);
      if (child.isPureVritual())
        m_isAbstract = true;
      break;
    default:
      break;
    }
  }
}

const std::string& Class::getClassName() const
{
    return m_className;
}

const std::vector<Field>& Class::getFieldsList() const
{
    return m_fields;
}

const std::vector<Method>& Class::getMethodsList() const
{
    return m_methods;
}

const std::string& Class::getSuperClassName() const
{
  return m_superClassName;
}
