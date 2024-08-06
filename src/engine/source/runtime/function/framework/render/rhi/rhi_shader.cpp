#include "rhi_shader.h"

namespace GEngine {

std::unordered_map<std::string, std::shared_ptr<RHIShader>>
    RHIShaders::m_shaderMap;
}