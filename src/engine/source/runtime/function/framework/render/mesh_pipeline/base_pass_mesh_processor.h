#pragma once

#include "function/framework/render/mesh_pipeline/mesh_pass_processor.h"
namespace GEngine {

class BasePassMeshProcessor : public MeshPassProcessor {
public:
 BasePassMeshProcessor(MeshPassType type, const std::shared_ptr<Scene>& scene,
                       const std::shared_ptr<SceneView>& sceneView)
     : MeshPassProcessor(type, scene, sceneView) {}

 virtual void addMeshBatch(
     const MeshBatch& meshBatch,
     const std::shared_ptr<PrimitiveSceneProxy>& sceneProxy) override;
};

}