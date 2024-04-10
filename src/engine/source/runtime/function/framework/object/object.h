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
  static auto otherClassID = T::getClass().getClassID();

  if(m_class == nullptr)
    getClass();
  Reflection::ClassDescriptor *thisClass = m_class;
  
  for (auto *curr = thisClass; curr != nullptr; curr = curr->getSuperClass()) {
    if (curr->getClassID() == otherClassID)
      return true;
  }
  return false;
}

}  // namespace GEngine