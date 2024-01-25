#include "object.h"

namespace GEngine {
Reflection::ClassDescriptor& GObject::getClass() {
  if (m_class == nullptr) {
    auto* thisClass =
        &Reflection::Registry::instance().getClass(getClassName());
    m_class = thisClass;
    return *m_class;
  } else
    return *m_class;
}

}  // namespace GEngine