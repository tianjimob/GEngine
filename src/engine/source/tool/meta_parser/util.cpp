#include "util.h"
#include <iostream>

namespace Util {

void toString(const CXString &str, std::string &output) {
  auto cstr = clang_getCString(str);
  if (cstr != nullptr) {
    output = cstr;
    clang_disposeString(str);
  }
}

} // namespace Util