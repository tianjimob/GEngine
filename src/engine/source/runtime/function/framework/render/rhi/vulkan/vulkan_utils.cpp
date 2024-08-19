#include "vulkan_utils.h"

#include "function/framework/render/core/shaders.h"
#include "function/framework/render/rhi/rhi_type.h"
#include "vulkan/vulkan_core.h"

namespace GEngine {

namespace VulkanUtils {

static VkFormat VertexElementTypeToFormat[] = {
    VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_SFLOAT};

VkFormat getVkFormat(VertexElementType type) {
  return VertexElementTypeToFormat[static_cast<int>(type)];
}

static VkPrimitiveTopology PrimitiveTypeToTopology[] = {
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};

VkPrimitiveTopology getVkPrimitiveTopology(RHIPrimitiveType type) {
  return PrimitiveTypeToTopology[static_cast<int>(type)];
}

static VkDescriptorType ShaderParametersTypeToDescriptor[] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER};
VkDescriptorType getVkDescriptorType(ShaderParametersType type) {
  return ShaderParametersTypeToDescriptor[static_cast<int>(type)];
}

VkAttachmentLoadOp getVkAttachmentLoadOp(AttachmentLoadOp loadOp) {
  switch (loadOp) {
    case AttachmentLoadOp::Load:
      return VK_ATTACHMENT_LOAD_OP_LOAD;
      break;
    case AttachmentLoadOp::Clear:
      return VK_ATTACHMENT_LOAD_OP_CLEAR;
      break;
    case AttachmentLoadOp::DontCare:
      return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      break;
    default:
      break;
  }
}

VkAttachmentStoreOp getVkAttachmentStoreOp(AttachmentStoreOp storeOp) {
  switch (storeOp) {
    case AttachmentStoreOp::Store:
      return VK_ATTACHMENT_STORE_OP_STORE;
      break;
    case AttachmentStoreOp::DontCare:
      return VK_ATTACHMENT_STORE_OP_DONT_CARE;
      break;
    default:
      break;
  }
}

VkCullModeFlags getVkCullModeFlags(CullMode mode) {
  switch (mode) {
    case CullMode::None:
      return VK_CULL_MODE_NONE;
      break;
    case CullMode::Front:
      return VK_CULL_MODE_FRONT_BIT;
      break;
    case CullMode::Back:
      return VK_CULL_MODE_BACK_BIT;
      break;
    default:
      break;
  }
}

VkPolygonMode getVkPolygonMode(PolygonMode mode) {
  switch (mode) {
    case PolygonMode::Point:
      return VK_POLYGON_MODE_POINT;
      break;
    case PolygonMode::line:
      return VK_POLYGON_MODE_LINE;
      break;
    case PolygonMode::Fill:
      return VK_POLYGON_MODE_FILL;
      break;
    default:
      break;
  }
}

static VkBlendFactor BlendFactorToVk[] = {
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_SRC_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    VK_BLEND_FACTOR_DST_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_FACTOR_DST_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    VK_BLEND_FACTOR_CONSTANT_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    VK_BLEND_FACTOR_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
    VK_BLEND_FACTOR_SRC1_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
    VK_BLEND_FACTOR_SRC1_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA};

VkBlendFactor getVkBlendFactor(BlendFactor factor) {
  return BlendFactorToVk[static_cast<int>(factor)];
}

static VkBlendOp BlendOpToVk[] = {VK_BLEND_OP_ADD, VK_BLEND_OP_SUBTRACT,
                                  VK_BLEND_OP_REVERSE_SUBTRACT, VK_BLEND_OP_MIN,
                                  VK_BLEND_OP_MAX};

VkBlendOp getVkBlendOp(BlendOperator op) {
  return BlendOpToVk[static_cast<int>(op)];
}

VkColorComponentFlags getVkColorComponentFlags(ColorWriteMask mask) {
  return static_cast<int>(mask);
}

}  // namespace VulkanUtils

}  // namespace GEngine