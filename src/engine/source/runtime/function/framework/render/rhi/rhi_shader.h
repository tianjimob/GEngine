#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace GEngine {

class RHIShaders {
public:
  static void init();
  static std::shared_ptr<RHIShader> getShader(const std::string &shaderName) {
    if (auto it = m_shaderMap.find(shaderName); it != m_shaderMap.end()) {
      return it->second;
    }
    return nullptr;
  }
private:
  static std::unordered_map<std::string, std::shared_ptr<RHIShader>> m_shaderMap;
};

}