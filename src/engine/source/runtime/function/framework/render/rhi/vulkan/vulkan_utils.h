#pragma once

#include "function/framework/render/core/shaders.h"
#include "function/framework/render/mesh_pipeline/vertex_streams.h"
#include "function/framework/render/rhi/rhi_type.h"
#include "vulkan/vulkan_core.h"
namespace GEngine {

namespace VulkanUtils {

VkFormat getVkFormat(VertexElementType type);

VkPrimitiveTopology getVkPrimitiveTopology(RHIPrimitiveType type);

VkDescriptorType getVkDescriptorType(ShaderParametersType type);

VkAttachmentLoadOp getVkAttachmentLoadOp(AttachmentLoadOp loadOp);

VkAttachmentStoreOp getVkAttachmentStoreOp(AttachmentStoreOp storeOp);

VkCullModeFlags getVkCullModeFlags(CullMode mode);

VkPolygonMode getVkPolygonMode(PolygonMode mode);

VkBlendFactor getVkBlendFactor(BlendFactor factor);

VkBlendOp getVkBlendOp(BlendOperator op);

VkColorComponentFlags getVkColorComponentFlags(ColorWriteMask mask);

}

}