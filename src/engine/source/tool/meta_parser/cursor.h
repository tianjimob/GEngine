#pragma once

#include <vector>
#include <clang-c/Index.h>

#include "cursor_type.h"

class Cursor
{
public:
    typedef std::vector<Cursor> List;
    
    typedef CXCursorVisitor Visitor;

    Cursor(const CXCursor& handle);

    CXCursorKind getKind(void) const;

    std::string getSpelling(void) const;
    std::string getDisplayName(void) const;

    std::string getSourceFile(void) const;

    bool isDefinition(void) const;
    bool isPureVritual() const;

    CursorType getType() const;

    bool hasAnnotateAttr(const std::string &attr);
    bool isAnnotateAttr() const;
    bool hasAttributes() const; // if true has any annotate attribute

    List getChildren(void) const;
    void visitChildren(Visitor visitor, void* data = nullptr);

private:
    CXCursor m_handle;
};