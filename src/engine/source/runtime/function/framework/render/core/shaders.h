#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "function/framework/render/core/shader_type.h"

#define BEGIN_SHADER_PARAMETER_STRUCT()                                 \
  class alignas(16) Parameters {                                        \
   private:                                                             \
    enum class MemberId { Sequence = 0 };                               \
    using FuncPtr = void *;                                             \
    typedef FuncPtr (*MemberFunc)(MemberId,                             \
                                  std::vector<ShaderPrameterMember> &); \
    static FuncPtr appendMemberGetPrev(                                 \
        MemberId, std::vector<ShaderPrameterMember> &members) {         \
      return nullptr;                                                   \
    }                                                                   \
    typedef MemberId

#define SHADER_PARAMETER_UNIFORM_BUFFER(structType, typeName)                 \
  MemberId##typeName;                                                         \
                                                                              \
 public:                                                                      \
  alignas(structType) structType typeName;                                    \
                                                                              \
 private:                                                                     \
  enum class NextMemberId##typeName{                                          \
      Sequence = static_cast<int>(MemberId##typeName::Sequence) + 1};         \
  static FuncPtr appendMemberGetPrev(                                         \
      NextMemberId##typeName, std::vector<ShaderPrameterMember> &members) {   \
    members.emplace_back(static_cast<int>(MemberId##typeName::Sequence),      \
                         ShaderParametersType::UniformBuffer,                 \
                         ShaderTypeInfo<structType>::Elements,                \
                         sizeof(structType), offsetof(Parameters, typeName)); \
    FuncPtr (*prevFuncPtr)(MemberId##typeName,                                \
                           std::vector<ShaderPrameterMember> &);              \
    prevFuncPtr = appendMemberGetPrev;                                        \
    return reinterpret_cast<void *>(prevFuncPtr);                             \
  }                                                                           \
  typedef NextMemberId##typeName

#define SHADER_PARAMETER_STORAGE_BUFFER(structType, typeName)                 \
  MemberId##typeName;                                                         \
                                                                              \
 public:                                                                      \
  alignas(structType) structType typeName;                                    \
                                                                              \
 private:                                                                     \
  enum class NextMemberId##typeName{                                          \
      Sequence = static_cast<int>(MemberId##typeName::Sequence) + 1};         \
  static FuncPtr appendMemberGetPrev(                                         \
      NextMemberId##typeName, std::vector<ShaderPrameterMember> &members) {   \
    members.emplace_back(static_cast<int>(MemberId##typeName::Sequence),      \
                         ShaderParametersType::StorageBuffer,                 \
                         ShaderTypeInfo<structType>::Elements,                \
                         sizeof(structType), offsetof(Parameters, typeName)); \
    FuncPtr (*prevFuncPtr)(MemberId##typeName,                                \
                           std::vector<ShaderPrameterMember> &);              \
    prevFuncPtr = appendMemberGetPrev;                                        \
    return reinterpret_cast<void *>(prevFuncPtr);                             \
  }                                                                           \
  typedef NextMemberId##typeName

#define END_SHADER_PARAMETER_STRUCT()                                          \
  LastMemberId;                                                                \
                                                                               \
 public:                                                                       \
  static std::vector<ShaderPrameterMember> getMembers() {                      \
    std::vector<ShaderPrameterMember> members;                                 \
    FuncPtr (*lastFuncPtr)(LastMemberId, std::vector<ShaderPrameterMember> &); \
    lastFuncPtr = appendMemberGetPrev;                                         \
    FuncPtr ptr = reinterpret_cast<void *>(lastFuncPtr);                       \
    do {                                                                       \
      ptr = reinterpret_cast<MemberFunc>(ptr)(MemberId(), members);            \
    } while (ptr);                                                             \
    members.shrink_to_fit();                                                   \
    std::reverse(members.begin(), members.end());                              \
    return members;                                                            \
  }                                                                            \
  }                                                                            \
  ;

namespace GEngine {

class Shader {};

enum class ShaderParametersType {
  UniformBuffer = 0,
  StorageBuffer,
  MaxNumType
};

struct ShaderPrameterMember {
  ShaderPrameterMember(uint32_t binding, ShaderParametersType type,
                       uint32_t elements, uint32_t size, uint32_t offset)
      : binding(binding),
        type(type),
        elements(elements),
        size(size),
        offset(offset) {}

  uint32_t binding;
  ShaderParametersType type;
  uint32_t elements;
  uint32_t size;
  uint32_t offset;
};

class MemcpyCS : public Shader {
 public:
  BEGIN_SHADER_PARAMETER_STRUCT()
  SHADER_PARAMETER_UNIFORM_BUFFER(MemcpyInfo, memcpyInfo)
  SHADER_PARAMETER_STORAGE_BUFFER(RHIBuffer*, ssboSrc)
  SHADER_PARAMETER_STORAGE_BUFFER(RHIBuffer*, ssboDst)
  END_SHADER_PARAMETER_STRUCT()
};

class PrimitiveSceneCS : public Shader {
public:
  BEGIN_SHADER_PARAMETER_STRUCT()
  SHADER_PARAMETER_UNIFORM_BUFFER(uint32_t, updateSize)
  SHADER_PARAMETER_STORAGE_BUFFER(RHIBuffer*, ssboSrc)
  SHADER_PARAMETER_STORAGE_BUFFER(RHIBuffer*, ssboDst)
  END_SHADER_PARAMETER_STRUCT()
};

}  // namespace GEngine