#include "depth_pass_mesh_processor.h"
#include "function/framework/render/rhi/rhi_type.h"

namespace GEngine {

DepthPassMeshProcessor::DepthPassMeshProcessor(
    MeshPassType type, const std::shared_ptr<Scene> &scene,
    const std::shared_ptr<SceneView> &sceneView)
    : MeshPassProcessor(type, scene, sceneView) {
  // m_renderState.depthStencilState = enable DepthTest, <= Pass
  //   m_renderState.blendState = ColorWriteMask::None
}

void DepthPassMeshProcessor::addMeshBatch(
    const MeshBatch &meshBatch,
    const std::shared_ptr<PrimitiveSceneProxy> &sceneProxy) {
    }

}  // namespace GEngine