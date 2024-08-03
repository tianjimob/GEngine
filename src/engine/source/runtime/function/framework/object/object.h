#pragma once

#include <memory>
#include <string_view>
#include <type_traits>

#include "core/reflection/reflection.h"
#include "core/misc/json.h"

namespace GEngine {
class GObject : public std::enable_shared_from_this<GObject> {
 public:
  virtual ~GObject() = default;

  virtual std::string_view getClassName() = 0;
  virtual void dynamicSerializeFromJson(const Json &jsonContext, GObject* outer) = 0;

  Reflection::ClassDescriptor &getClass();

  // called after Serializer::read
  virtual void postLoad(std::weak_ptr<GObject> parentObject) {}
  // called in Serializer::read
  virtual void onSerializeRead() {}

  template <typename T> bool isA();
  bool is(Reflection::ClassDescriptor& classDesc);

  template <typename T> std::shared_ptr<T> createSubobject();

  GObject *getOuter() { return m_outer; }
  void setOuter(GObject *outer) { m_outer = outer; }

private:
  GObject* m_outer{nullptr};
  Reflection::ClassDescriptor *m_class{nullptr};
  Reflection::ClassDescriptor *m_baseClass{nullptr};
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

template <typename T> std::shared_ptr<T> GObject::createSubobject() {
  static_assert(std::is_base_of_v<GObject, T>, "T must be child class of GObject");
  std::shared_ptr<T> newObject = std::make_shared<T>();
  if (newObject) {
    newObject->m_outer = weak_from_this();
  }
  return newObject;
}

}  // namespace GEngine