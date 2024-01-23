#pragma once

#include <clang-c/Index.h>

#include <string>

namespace Util {
    void toString(const CXString& str, std::string& output);
}