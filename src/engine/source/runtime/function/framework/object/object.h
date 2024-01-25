#pragma once

#include <string_view>

#include "core/reflection/reflection.h"

namespace GEngine {
class GObject {
 public:
  virtual ~GObject() = default;

  virtual std::string_view getClassName() = 0;

  Reflection::ClassDescriptor &getClass();

  template <typename T>
  bool isA();

 private:
  Reflection::ClassDescriptor *m_class { nullptr };
  Reflection::ClassDescriptor *m_baseClass { nullptr };
};

template <typename T> bool GObject::isA() {
  static Reflection::ClassDescriptor *otherClass = &T::getClass();

  Reflection::ClassDescriptor *thisClass = &getClass();
  
  for (auto *curr = thisClass; curr != nullptr; curr = curr->getSuperClass()) {
    if (curr == otherClass)
      return true;
  }
  return false;
}

}  // namespace GEngine