#include "cursor.h"
#include "cursor_type.h"
#include "util.h"

CursorType::CursorType(const CXType& handle) : m_handle(handle) {}

std::string CursorType::GetDisplayName(void) const
{
    std::string displayName;
    Util::toString(clang_getTypeSpelling(m_handle), displayName);
    return displayName;
}

int CursorType::GetArgumentCount(void) const { return clang_getNumArgTypes(m_handle); }

CursorType CursorType::GetArgument(unsigned index) const { return clang_getArgType(m_handle, index); }

CursorType CursorType::GetCanonicalType(void) const { return clang_getCanonicalType(m_handle); }

Cursor CursorType::GetDeclaration(void) const { return clang_getTypeDeclaration(m_handle); }

CXTypeKind CursorType::GetKind(void) const { return m_handle.kind; }

bool CursorType::isArray() const
{
    return clang_getArraySize(m_handle) != -1;
}

size_t CursorType::getArraySize() const
{
    return clang_getArraySize(m_handle);
}

bool CursorType::IsConst(void) const { return clang_isConstQualifiedType(m_handle) ? true : false; }

CursorType CursorType::getElementType() const
{
    return clang_getElementType(m_handle);
}

CursorType CursorType::getTemplateArgumentAsType(unsigned i) const
{
    return clang_Type_getTemplateArgumentAsType(m_handle, i);
}