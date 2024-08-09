#pragma once

#include <memory>
#include <string_view>
#include <type_traits>

#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "core/misc/json.h"
#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"


namespace GEngine {

class Serializer {
 public:
  template <typename T>
  static Json write(const std::shared_ptr<T> &instance) {
    static_assert(std::is_base_of_v<GObject, T>,
                  "T must be child class of GObject");
    if (instance) {
      return {{"$typeName", instance->getClassName()},
              {"$context", Serializer::write(*instance)}};
    }
    return {};
  }

  template <typename T>
  static std::shared_ptr<T> &read(const Json &jsonContext,
                                  std::shared_ptr<T> &instance,
                                  GObject *outer) {
    static_assert(std::is_base_of_v<GObject, T>,
                  "T must be child class of GObject");
    auto &classDesc = GET_CLASS(jsonContext["$typeName"]);
    if (!instance) {
      if (classDesc.isSubclassOf(T::getClass())) {
        instance = std::shared_ptr<T>{(T *)(classDesc.construct())};
      }
    }
    if (instance) {
      instance->dynamicSerializeFromJson(jsonContext["$context"], outer);
    }
    return instance;
  }

  template <typename T> static Json write(const T &instance);

  template <typename T>
  static T &read(const Json &json_context, T &instance, GObject *outer);

};

// implementation of base types
template <>
Json Serializer::write(const char &instance);
template <>
char &Serializer::read(const Json &json_context, char &instance,
                       GObject *outer);

template <>
Json Serializer::write(const int &instance);
template <>
int &Serializer::read(const Json &json_context, int &instance, GObject *outer);

template <>
Json Serializer::write(const unsigned int &instance);
template <>
unsigned int &Serializer::read(const Json &json_context, unsigned int &instance,
                               GObject *outer);

template <>
Json Serializer::write(const float &instance);
template <>
float &Serializer::read(const Json &json_context, float &instance,
                        GObject *outer);

template <>
Json Serializer::write(const double &instance);
template <>
double &Serializer::read(const Json &json_context, double &instance,
                         GObject *outer);

template <>
Json Serializer::write(const bool &instance);
template <>
bool &Serializer::read(const Json &json_context, bool &instance,
                       GObject *outer);

template <>
Json Serializer::write(const std::string &instance);
template <>
std::string &Serializer::read(const Json &json_context, std::string &instance,
                              GObject *outer);

template <>
Json Serializer::write(const Vector2 &instance);
template <>
Vector2 &Serializer::read(const Json &json_context, Vector2 &instance,
                          GObject *outer);

template <>
Json Serializer::write(const Vector3 &instance);
template <>
Vector3 &Serializer::read(const Json &json_context, Vector3 &instance,
                          GObject *outer);

template <>
Json Serializer::write(const Vector4 &instance);
template <>
Vector4 &Serializer::read(const Json &json_context, Vector4 &instance,
                          GObject *outer);

template <>
Json Serializer::write(Reflection::ClassDescriptor * const &instance);
template <>
Reflection::ClassDescriptor *&Serializer::read(
    const Json &json_context, Reflection::ClassDescriptor *&instance,
    GObject *outer);

}  // namespace GEngine