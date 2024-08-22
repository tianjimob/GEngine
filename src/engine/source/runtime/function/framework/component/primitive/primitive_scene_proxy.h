#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "core/math/bounds.h"
#include "core/math/box_sphere_bounds.h"
#include "core/math/matrix4.h"
#include "core/math/transform.h"
#include "core/math/vector3.h"

namespace GEngine {

class PrimitiveSceneInfo;
class PrimitiveComponent;

struct InstanceSceneData {
  Matrix4x4 localToPrimitive;

  Matrix4x4 computeLocalToWorld(const Matrix4x4 primitiveToWorld) const {
    return primitiveToWorld * localToPrimitive;
  }
};

class PrimitiveSceneProxy {
 public:
  PrimitiveSceneProxy(PrimitiveComponent& primitive);
  void setSceneInfo(std::shared_ptr<PrimitiveSceneInfo>& sceneInfo) {
    m_sceneInfo = sceneInfo;
  }

  const Matrix4x4& getLocalToWorld() const { return m_localToWorld; }
  const Vector3& getActorPosition() const { return m_actorPosition; }
  const BoxSphereBounds& getWorldBounds() const { return m_worldBounds; }
  const BoxSphereBounds& getLocalBounds() const { return m_localBounds; }
  int32_t getInstanceSceneDataOffset() const {
    return m_instanceSceneDataOffset;
  }
  int32_t getNumInstanceSceneDataEntries() const {
    return m_numInstanceSceneDataEntries;
  }
  const auto& getInstanceSceneData() const { return m_instanceSceneData; }
  const auto &getInstanceLocalBounds() const { return m_instanceLocalBounds; }
  const InstanceSceneData& getInstanceSceneData(uint32_t instanceIndex) const {
    return m_instanceSceneData[instanceIndex];
  }
  const Bounds &getInstanceLocalBounds(uint32_t instanceIndex) const {
    return m_instanceLocalBounds[instanceIndex];
  }

 private:
  uint32_t m_primitiveComponentId;
  std::weak_ptr<PrimitiveSceneInfo> m_sceneInfo;

  /** The primitive's local to world transform. */
  Matrix4x4 m_localToWorld;

  /** The component's actor's position. */
  Vector3 m_actorPosition;

  // primitive's world space bounds
  BoxSphereBounds m_worldBounds;

  // primitive's local space bounds
  BoxSphereBounds m_localBounds;

  /** Offset into the scene's instance scene data buffer, when GPUScene is
   * enabled. */
  int32_t m_instanceSceneDataOffset;

  /** Number of entries in the scene's instance scene data buffer. */
  int32_t m_numInstanceSceneDataEntries;

  std::vector<InstanceSceneData> m_instanceSceneData;

  std::vector<Bounds> m_instanceLocalBounds;
};

}  // namespace GEngine