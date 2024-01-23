#pragma once

#include <any>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#define REFLECTION_CLASS(className)                                            \
  namespace Reflection {                                                       \
  namespace Register {                                                         \
  class Register##className;                                                   \
  }                                                                            \
  };
#define REFLECTION_STRUCT(structName) REFLECTION_CLASS(structName)
#define REFLECTION_ENUM(enumName)

#if defined(__META_PARSER__)
#define META_FIELD(...) __attribute__((annotate("meta-field", #__VA_ARGS__)))
#define META_FUNCTION(...)                                                     \
  __attribute__((annotate("meta-method", #__VA_ARGS__)))
#define CLASS_ARGS(className, ...)                                             \
  class __attribute__((annotate("meta-class", #__VA_ARGS__))) className
#define STRUCT_ARGS(structName, ...)                                           \
  struct __attribute__((annotate("meta-struct", #__VA_ARGS__))) structName
#define ENUM_ARGS(enumName, ...)                                               \
  enum class __attribute__((annotate("meta-enum", #__VA_ARGS__))) enumName
#else
#define META_FIELD(...)
#define META_FUNCTION(...)
#define CLASS_ARGS(className, ...) class className
#define STRUCT_ARGS(structName, ...) struct structName
#define ENUM_ARGS(enumName, ...) enum class enumName
#endif // __REFLECTION_PARSER__

#define CLASS(className, ...)                                                  \
  REFLECTION_CLASS(className)                                                  \
  CLASS_ARGS(className, #__VA_ARGS__)

#define STRUCT(structName, ...)                                                \
  REFLECTION_STRUCT(structName)                                                \
  STRUCT_ARGS(structName, #__VA_ARGS__)

#define ENUM(enumName, ...)                                                    \
  REFLECTION_ENUM(enumName)                                                    \
  ENUM_ARGS(enumName, #__VA_ARGS__)

#define REFLECTION_BODY(className)                                             \
  friend class ::GEngine::Reflection::Register::Register##className;

#define REGISTER_CLASS(classDescriptorBuilder)                                 \
  ::GEngine::Reflection::Registry::instance().registerClass(                   \
      std::move(classDescriptorBuilder.getClassDescriptor()));

#define REGISTER_ENUM(enumDescriptorBuilder)                                   \
  ::GEngine::Reflection::Registry::instance().registerEnum(                    \
      std::move(enumDescriptorBuilder.getEnumDescriptor()));

namespace GEngine {
namespace Reflection {

using SetterFunction = std::function<void(void *, void *)>;
using GetterFunction = std::function<void *(void *)>;
using ArraySizeFunction = std::function<size_t(const void *)>;
using ArraySetterFunction = std::function<void(void *, void *, size_t)>;
using ArrayGetterFunction = std::function<void *(void *, size_t)>;
using MethodFunction = std::function<std::any(std::any)>;

class ClassDescriptorBuilder;

class Field {
public:
  template <typename Var, typename Class>
  Field(const std::string &fieldName, const std::string &fieldTypeName,
        Var Class::*var);

  const std::string &getFieldName() const;
  const std::string &getFieldTypeName() const;

  void set(void *instance, void *value) const;
  void *get(void *instance) const;

  bool isEnum() const;
  bool isArray() const;

  size_t getOffsetInClass() const;

  const std::string &getElementTypeName() const;
  void setElement(void *instance, void *value, size_t index) const;
  void *getElement(void *instance, size_t index) const;
  size_t getSize(void *instance) const;

private:
  friend class ClassDescriptorBuilder;

  std::string m_fieldName;
  std::string m_fieldTypeName;
  SetterFunction m_setter;
  GetterFunction m_getter;

  bool m_isEnum : 4; // getFieldTypeName() will return enum name if true

  bool m_isArray : 4;
  // valid if field is array
  std::string m_elementTypeName;
  ArraySizeFunction m_arraySizeGetter;
  ArraySetterFunction m_arraySetter;
  ArrayGetterFunction m_arrayGetter;

  size_t m_offsetInClass;
};

template <typename Var, typename Class>
inline size_t getOffsetOfPointerToField(Var Class::*var) {
  union {
    Var Class::*v;
    void *p;
  };
  p = nullptr;
  v = var;
  return size_t(p);
}

template <typename Var, typename Class>
Field::Field(const std::string &fieldName, const std::string &fieldTypeName,
             Var Class::*var)
    : m_fieldName(fieldName), m_fieldTypeName(fieldTypeName) {

  m_setter = [var](void *instance, void *value) {
    if constexpr (!std::is_array_v<Var>) {
      static_cast<Class *>(instance)->*var = *static_cast<Var *>(value);
    } else {
      std::cerr << "this field is pure array type!" << std::endl;
    }
  };

  m_getter = [var](void *instance) {
    return static_cast<void *>(&(static_cast<Class *>(instance)->*var));
  };

  m_offsetInClass = getOffsetOfPointerToField(var);
}

class Method {
public:
  template <typename Return, typename Class, typename... Args>
  Method(const std::string &methodName, Return (Class::*func)(Args...));

  template <typename Class, typename... Args>
  Method(const std::string &methodName, void (Class::*func)(Args...));

  template <typename Return, typename Class, typename... Args>
  Method(const std::string &methodName, Return (Class::*func)(Args...) const);

  template <typename Class, typename... Args>
  Method(const std::string &methodName, void (Class::*func)(Args...) const);

  const std::string &getMethodName() const;

  template <typename Class, typename... Args>
  std::any invoke(Class &instance, Args... args);

private:
  std::string m_methodName;
  MethodFunction m_method;
  bool m_isConst;
};

template <typename Return, typename Class, typename... Args>
Method::Method(const std::string &methodName, Return (Class::*func)(Args...))
    : m_methodName(methodName) {
  m_method = [func](std::any args) -> std::any {
    using TupleArgs = std::tuple<Class &, Args...>;
    TupleArgs *tupleArgs = std::any_cast<TupleArgs *>(args);
    return std::apply(func, *tupleArgs);
  };
  m_isConst = false;
}

template <typename Class, typename... Args>
Method::Method(const std::string &methodName, void (Class::*func)(Args...))
    : m_methodName(methodName) {
  m_method = [func](std::any args) -> std::any {
    using TupleArgs = std::tuple<Class &, Args...>;
    TupleArgs *tupleArgs = std::any_cast<TupleArgs *>(args);
    std::apply(func, *tupleArgs);
    return {};
  };
  m_isConst = false;
}

template <typename Return, typename Class, typename... Args>
Method::Method(const std::string &methodName,
               Return (Class::*func)(Args...) const)
    : m_methodName(methodName) {
  m_method = [func](std::any args) -> std::any {
    using TupleArgs = std::tuple<const Class &, Args...>;
    TupleArgs *tupleArgs = std::any_cast<TupleArgs *>(args);
    return std::apply(func, *tupleArgs);
  };
  m_isConst = true;
}

template <typename Class, typename... Args>
Method::Method(const std::string &methodName,
               void (Class::*func)(Args...) const)
    : m_methodName(methodName) {
  m_method = [func](std::any args) -> std::any {
    using TupleArgs = std::tuple<const Class &, Args...>;
    TupleArgs *tupleArgs = std::any_cast<TupleArgs *>(args);
    std::apply(func, *tupleArgs);
    return {};
  };
  m_isConst = true;
}

template <typename Class, typename... Args>
std::any Method::invoke(Class &instance, Args... args) {
  if (m_isConst) {
    auto tup =
        std::make_tuple(std::reference_wrapper<const Class>(instance), args...);
    return m_method(&tup);
  }
  auto tup = std::make_tuple(std::reference_wrapper<Class>(instance), args...);
  return m_method(&tup);
}

class ClassDescriptor {
public:
  ClassDescriptor(const std::string &className);

  const std::string &getClassName() const;

  const std::vector<Field> &getFieldsList() const;
  const std::vector<Method> &getMethodsList() const;

  std::optional<Field> getFieldByName(const std::string &name);
  std::optional<Method> getMethodByName(const std::string &name);

private:
  friend class ClassDescriptorBuilder;

  std::string m_className;
  std::vector<Field> m_fields;
  std::vector<Method> m_methods;
};

class EnumDescriptor {
public:
  EnumDescriptor(const std::string &enumName);

  const std::string &getEnumName() const;

  const std::vector<std::pair<std::string, int64_t>> &getEnumsList() const;

  std::optional<std::string> getNameByValue(int64_t value) const;
  std::optional<int64_t> getValueByName(const std::string &name) const;

private:
  friend class EnumDescriptorBuilder;

  std::string m_enumName;
  std::vector<std::pair<std::string, int64_t>> m_enums;
};

class ClassDescriptorBuilder {
public:
  ClassDescriptorBuilder(const std::string &name);

  template <typename Var, typename Class>
  ClassDescriptorBuilder &addField(const std::string &fieldName,
                                   const std::string &fieldTypeName,
                                   Var Class::*var, bool isEnum = false);

  template <typename Var, typename Class>
  ClassDescriptorBuilder &
  addField(const std::string &fieldName, const std::string &fieldTypeName,
           Var Class::*var, const std::string &elementTypeName,
           ArraySizeFunction sizeFunc, ArraySetterFunction setter,
           ArrayGetterFunction getter, bool isEnum = false);

  template <typename Method>
  ClassDescriptorBuilder &addMethod(const std::string &methodName,
                                    Method method);

  std::unique_ptr<ClassDescriptor> &getClassDescriptor();

private:
  std::unique_ptr<ClassDescriptor> m_classDescriptor;
};

template <typename Var, typename Class>
ClassDescriptorBuilder &
ClassDescriptorBuilder::addField(const std::string &fieldName,
                                 const std::string &fieldTypeName,
                                 Var Class::*var, bool isEnum) {
  Field &field =
      m_classDescriptor->m_fields.emplace_back(fieldName, fieldTypeName, var);
  field.m_isEnum = isEnum;
  field.m_isArray = false;
  return *this;
}

template <typename Var, typename Class>
ClassDescriptorBuilder &ClassDescriptorBuilder::addField(
    const std::string &fieldName, const std::string &fieldTypeName,
    Var Class::*var, const std::string &elementTypeName,
    ArraySizeFunction sizeFunc, ArraySetterFunction setter,
    ArrayGetterFunction getter, bool isEnum) {
  Field &field =
      m_classDescriptor->m_fields.emplace_back(fieldName, fieldTypeName, var);
  field.m_isEnum = isEnum;
  field.m_isArray = true;
  field.m_arraySizeGetter = sizeFunc;
  field.m_arraySetter = setter;
  field.m_arrayGetter = getter;
  return *this;
}

template <typename Method>
ClassDescriptorBuilder &
ClassDescriptorBuilder::addMethod(const std::string &methodName,
                                  Method method) {
  m_classDescriptor->m_methods.emplace_back(methodName, method);
  return *this;
}

class EnumDescriptorBuilder {
public:
  EnumDescriptorBuilder(const std::string &name);

  EnumDescriptorBuilder &addEnum(const std::string &name, int64_t value);

  std::unique_ptr<EnumDescriptor> &getEnumDescriptor();

private:
  std::unique_ptr<EnumDescriptor> m_enumDescriptor;
};

class Registry {
public:
  static Registry &instance();

  void registerClass(std::unique_ptr<ClassDescriptor> classDescriptor);
  void registerEnum(std::unique_ptr<EnumDescriptor> enumDescriptor);
  ClassDescriptor &getClass(const std::string &name);
  EnumDescriptor &getEnum(const std::string &name);

public:
  std::map<std::string, std::unique_ptr<ClassDescriptor>> m_classMap;
  std::map<std::string, std::unique_ptr<EnumDescriptor>> m_enumMap;
};

void autoRegisterAll();

} // namespace Reflection

} // namespace GEngine