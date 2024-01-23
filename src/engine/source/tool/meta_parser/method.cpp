#include "method.h"

Method::Method(const Cursor &cursor) : m_methodName(cursor.getSpelling()) {}

const std::string Method::getMethodName() const
{
    return m_methodName;
}
