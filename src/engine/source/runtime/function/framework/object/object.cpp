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

bool GObject::is(Reflection::ClassDescriptor &classDesc) {
  if (m_class->getClassID() == classDesc.getClassID())
    return true;
  else
   return false;
}

}  // namespace GEngine