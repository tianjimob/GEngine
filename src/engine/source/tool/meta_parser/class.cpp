#include "class.h"


Class::Class(const Cursor& cursor):m_className(cursor.getType().GetDisplayName())
{
  for (auto &child : cursor.getChildren()) {
    switch (child.getKind()) {
    case CXCursor_CXXBaseSpecifier:
      m_baseClassNames.emplace_back(child.getType().GetDisplayName());
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
