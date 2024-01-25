#include "class.h"
#include <cassert>


Class::Class(const Cursor& cursor):m_className(cursor.getType().GetDisplayName())
{
  for (auto &child : cursor.getChildren()) {
    switch (child.getKind()) {
    case CXCursor_CXXBaseSpecifier:
      if(m_superClassName.empty())
        m_superClassName = child.getType().GetDisplayName();
      else
       assert(0 && "meta parser cannot support multiple inheritance");
      break;
    case CXCursor_FieldDecl:
      if(child.hasAnnotateAttr("meta-field"))
        m_fields.emplace_back(child);
      break;
    case CXCursor_CXXMethod:
      if(child.hasAnnotateAttr("meta-method"))
        m_methods.emplace_back(child);
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
