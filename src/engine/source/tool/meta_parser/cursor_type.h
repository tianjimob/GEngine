#pragma once

#include <string>
#include <clang-c/Index.h>

class Cursor;

class CursorType
{
public:
    CursorType(const CXType& handle);

    std::string GetDisplayName(void) const;

    int GetArgumentCount(void) const;

    CursorType GetArgument(unsigned index) const;

    CursorType GetCanonicalType(void) const;

    Cursor GetDeclaration(void) const;

    CXTypeKind GetKind(void) const;

    bool isArray() const;

    size_t getArraySize() const;

    bool IsConst(void) const;

    CursorType getElementType() const;

    CursorType getTemplateArgumentAsType(unsigned i) const;

private:
    CXType m_handle;
};