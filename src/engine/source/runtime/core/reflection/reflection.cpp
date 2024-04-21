#include "reflection.h"
#include <algorithm>
#include <atomic>
#include <memory>
#include <string>
#include <utility>

namespace GEngine {

namespace Reflection {

const std::string &Field::getFieldName() const { return m_fieldName; }

const std::string &Field::getFieldTypeName() const { return m_fieldTypeName; }

void Field::set(void *instance, void *value) const { m_setter(instance, value); }

void *Field::get(void *instance) const { return m_getter(instance); }

bool Field::isEnum() const
{
  return m_isEnum;
}

bool Field::isArray() const { return m_isArray; }

size_t Field::getOffsetInClass() const {
  return m_offsetInClass;
}

const std::string& Field::getElementTypeName() const
{
  return m_elementTypeName;
}

void Field::setElement(void *instance, void *value, size_t index) const
{
  m_arraySetter(instance, value, index);
}

void* Field::getElement(void *instance, size_t index) const
{
  return m_arrayGetter(instance, index);
}

size_t Field::getSize(void *instance) const
{
  return m_arraySizeGetter(instance);
}

const std::string &Method::getMethodName() const { return m_methodName; }

std::atomic_uint32_t ClassDescriptor::s_classId{0};

ClassDescriptor::ClassDescriptor(const std::string &className)
    : m_className(className), m_classId(s_classId++) {}

const std::string &ClassDescriptor::getClassName() const { return m_className; }

const std::vector<Field> &ClassDescriptor::getFieldsList() const {
  return m_fields;
}

const std::vector<Method> &ClassDescriptor::getMethodsList() const {
  return m_methods;
}

std::optional<Field> ClassDescriptor::getFieldByName(const std::string &name) {
  auto it = std::find_if(
      m_fields.begin(), m_fields.end(),
      [&name](const Field &field) { return name == field.getFieldName(); });
  if (it != m_fields.end())
    return *it;
  else
    return {};
}

std::optional<Method>
ClassDescriptor::getMethodByName(const std::string &name) {
  auto it = std::find_if(
      m_methods.begin(), m_methods.end(),
      [&name](const Method &method) { return name == method.getMethodName(); });
  if (it != m_methods.end())
    return *it;
  else
    return {};
}

const std::string &ClassDescriptor::getSuperClassName() const {
  return m_superClassName;
}

ClassDescriptor *ClassDescriptor::getSuperClass() { return m_superClass; }

bool ClassDescriptor::isSubclassOf(ClassDescriptor &base) {
  for (auto *curr = this; curr != nullptr; curr = curr->getSuperClass()) {
    if (curr->getClassID() == base.getClassID())
      return true;
  }
  return false;
}

EnumDescriptor::EnumDescriptor(const std::string &enumName)
    : m_enumName(enumName) {}

const std::string &EnumDescriptor::getEnumName() const { return m_enumName; }

const std::vector<std::pair<std::string, int64_t>> &
EnumDescriptor::getEnumsList() const {
  return m_enums;
}

std::optional<std::string> EnumDescriptor::getNameByValue(int64_t value) const {
  auto it = std::find_if(m_enums.begin(), m_enums.end(),
                         [value](const std::pair<std::string, int64_t> &e) {
                           return e.second == value;
                         });
  if (it != m_enums.end())
    return (*it).first;
  else
    return {};
}

std::optional<int64_t>
EnumDescriptor::getValueByName(const std::string &name) const {
  auto it = std::find_if(m_enums.begin(), m_enums.end(),
                         [&name](const std::pair<std::string, int64_t> &e) {
                           return e.first == name;
                         });
  if (it != m_enums.end())
    return (*it).second;
  else
    return {};
}

ClassDescriptorBuilder::ClassDescriptorBuilder(const std::string &name)
    : m_classDescriptor(std::make_unique<ClassDescriptor>(name)) {}

std::unique_ptr<ClassDescriptor> &ClassDescriptorBuilder::getClassDescriptor() {
  return m_classDescriptor;
}

ClassDescriptorBuilder &
ClassDescriptorBuilder::setSuperClassName(const std::string &superClassName) {
  m_classDescriptor->m_superClassName = superClassName;
  return *this;
}

EnumDescriptorBuilder::EnumDescriptorBuilder(const std::string &name)
    : m_enumDescriptor(std::make_unique<EnumDescriptor>(name)) {}

EnumDescriptorBuilder &EnumDescriptorBuilder::addEnum(const std::string &name,
                                                      int64_t value) {
  m_enumDescriptor->m_enums.emplace_back(name, value);
  return *this;
}

std::unique_ptr<EnumDescriptor>& EnumDescriptorBuilder::getEnumDescriptor()
{
  return m_enumDescriptor;
}

Registry &Registry::instance() {
  static Registry instance;
  return instance;
}

void Registry::registerClass(std::unique_ptr<ClassDescriptor> classDescriptor) {
  auto name = classDescriptor->getClassName();
  m_classMap[name] = std::move(classDescriptor);
}

void Registry::registerEnum(std::unique_ptr<EnumDescriptor> enumDescriptor)
{
  auto name = enumDescriptor->getEnumName();
  m_enumMap[name] = std::move(enumDescriptor);
}

// ClassDescriptor &Registry::getClass(const std::string &name) {
//   return *m_classMap[name];
// }

ClassDescriptor &Registry::getClass(std::string_view name) {
  return *m_classMap[name.data()];
}

EnumDescriptor& Registry::getEnum(const std::string &name)
{
  return *m_enumMap[name];
}

void autoSetupSuperClassInfo() {
  // register GEngine::GObject class
  ClassDescriptorBuilder classBuilder{"GEngine::GObject"};
  REGISTER_CLASS(classBuilder);

  auto &classMap = Registry::instance().m_classMap;
  for (auto &classPair : classMap) {
    auto &thisClass = classPair.second;
    auto &superClassName = thisClass->getSuperClassName();

    if (superClassName.empty()) {
      thisClass->m_superClass = nullptr;
      continue;
    }

    auto itSuperClass = classMap.find(superClassName);
    if (itSuperClass == classMap.end()) {
      thisClass->m_superClass = nullptr;
    } else {
      thisClass->m_superClass = itSuperClass->second.get();
    }
  }
}

} // namespace Reflection

} // namespace GEngine