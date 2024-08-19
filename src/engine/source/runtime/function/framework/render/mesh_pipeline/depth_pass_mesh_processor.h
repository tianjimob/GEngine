#pragma once

#include "function/framework/render/mesh_pipeline/mesh_pass_processor.h"
namespace GEngine {

class DepthPassMeshProcessor : public MeshPassProcessor {
public:
  DepthPassMeshProcessor(MeshPassType type, const std::shared_ptr<Scene>& scene,
                       const std::shared_ptr<SceneView>& sceneView);

 virtual void addMeshBatch(
     const MeshBatch& meshBatch,
     const std::shared_ptr<PrimitiveSceneProxy>& sceneProxy) override;

 private:
  MeshPassProcessorRenderState m_renderState;
};

}