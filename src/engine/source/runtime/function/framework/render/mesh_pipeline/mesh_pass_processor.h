#pragma once

#include "function/framework/render/mesh_pipeline/mesh_batch.h"
#include <memory>
namespace GEngine {

enum class MeshPassType { BasePass, LightPass };

class Scene;
class PrimitiveSceneProxy;
class SceneView;
class RHIBlendState;
class RHIDepthStencilState;

class  MeshPassProcessor {
public:
  MeshPassType meshPassType;
  const std::shared_ptr<Scene> scene;
  const std::shared_ptr<SceneView> sceneView;

  virtual ~MeshPassProcessor() = default;

  MeshPassProcessor(MeshPassType type, const std::shared_ptr<Scene>& scene,
                    const std::shared_ptr<SceneView>& sceneView)
      : meshPassType(type), scene(scene), sceneView(sceneView) {}

  virtual void addMeshBatch(const MeshBatch& meshBatch, const std::shared_ptr<PrimitiveSceneProxy>& sceneProxy) = 0;
};

struct MeshPassProcessorRenderState {
  std::shared_ptr<RHIDepthStencilState> depthStencilState;
  std::shared_ptr<RHIBlendState> blendState;
};

}