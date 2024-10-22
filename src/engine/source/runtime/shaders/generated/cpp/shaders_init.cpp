#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_shader.h"

#include "memcpycs_comp.h"
#include "primitivescenecs_comp.h"

namespace GEngine {

void RHIShaders::init() {
  m_shaderMap["MemcpyCS"] =
      GlobalRHI->createComputeShader(SHADER_CODE_MEMCPYCS_COMP);
  m_shaderMap["MemcpyCS"]->setMembers(MemcpyCS::Parameters::getMembers());

  m_shaderMap["PrimitiveSceneCS"] =
      GlobalRHI->createComputeShader(SHADER_CODE_PRIMITIVESCENECS_COMP);
  m_shaderMap["PrimitiveSceneCS"]->setMembers(PrimitiveSceneCS::Parameters::getMembers());
}

}