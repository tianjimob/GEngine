#pragma once

#include "nlohmann/json.hpp"
#include <memory>

namespace GEngine {
using Json = nlohmann::json;

class Serializer {
public:
 template <typename T> static Json write(const std::shared_ptr<T> &instance) {
   if (instance) {
     return Serializer::write(*instance);
   }
   return {};
 }

 template <typename T>
 static std::shared_ptr<T> &read(const Json &json_context,
                                 std::shared_ptr<T> &instance) {
   if (!instance) {
     instance = std::make_shared<T>();
     Serializer::read(json_context, *instance);
     return instance;
   }
   return instance;
 }

 template <typename T>
 static Json write(const T &instance);

 template <typename T>
 static T &read(const Json &json_context, T &instance);
};

// implementation of base types
template <> Json Serializer::write(const char &instance);
template <> char &Serializer::read(const Json &json_context, char &instance);

template <> Json Serializer::write(const int &instance);
template <> int &Serializer::read(const Json &json_context, int &instance);

template <> Json Serializer::write(const unsigned int &instance);
template <>
unsigned int &Serializer::read(const Json &json_context,
                               unsigned int &instance);

template <> Json Serializer::write(const float &instance);
template <> float &Serializer::read(const Json &json_context, float &instance);

template <> Json Serializer::write(const double &instance);
template <>
double &Serializer::read(const Json &json_context, double &instance);

template <> Json Serializer::write(const bool &instance);
template <> bool &Serializer::read(const Json &json_context, bool &instance);

template <> Json Serializer::write(const std::string &instance);
template <>
std::string &Serializer::read(const Json &json_context, std::string &instance);

} // namespace GEngine