#include "scene_data.h"
#include "core/math/vector4.h"
#include "function/framework/component/primitive/primitive_scene_proxy.h"
#include <cstdint>

namespace GEngine {

PrimitiveSceneData::PrimitiveSceneData(
    const PrimitiveSceneProxy *primitiveSceneProxy) {
  const auto& localToWorld = primitiveSceneProxy->getLocalToWorld().transpose();
  data[0] = *reinterpret_cast<const Vector4 *>(&localToWorld[0][0]);
  data[1] = *reinterpret_cast<const Vector4 *>(&localToWorld[1][0]);
  data[2] = *reinterpret_cast<const Vector4 *>(&localToWorld[2][0]);
  data[3] = *reinterpret_cast<const Vector4 *>(&localToWorld[3][0]);

  const auto& worldToLocal = localToWorld.inverse().transpose();
  data[4] = *reinterpret_cast<const Vector4 *>(&worldToLocal[0][0]);
  data[5] = *reinterpret_cast<const Vector4 *>(&worldToLocal[1][0]);
  data[6] = *reinterpret_cast<const Vector4 *>(&worldToLocal[2][0]);
  data[7] = *reinterpret_cast<const Vector4 *>(&worldToLocal[3][0]);

  const auto &actorWorldPosition = primitiveSceneProxy->getActorPosition();
  const auto &worldBounds = primitiveSceneProxy->getWorldBounds();
  const auto &localBounds = primitiveSceneProxy->getLocalBounds();
  data[8] = Vector4{actorWorldPosition, worldBounds.extent.x};
  data[9] = Vector4{worldBounds.origin, worldBounds.radius};
  data[10] = Vector4{localBounds.getBoxMin(), worldBounds.extent.y};
  data[11] = Vector4{localBounds.getBoxMax(), worldBounds.extent.z};

  data[12].x =
      static_cast<float>(primitiveSceneProxy->getInstanceSceneDataOffset());
  data[12].y =
      static_cast<float>(primitiveSceneProxy->getNumInstanceSceneDataEntries());
}

PrimitiveInstanceSceneData::PrimitiveInstanceSceneData(
    const PrimitiveSceneProxy *primitiveSceneProxy, uint32_t primitiveId, uint32_t instanceIndex) {
  const auto &primitiveToWorld = primitiveSceneProxy->getLocalToWorld().transpose();
  const auto &sceneData =
      primitiveSceneProxy->getInstanceSceneData(instanceIndex);
  const auto &localToWorld = sceneData.computeLocalToWorld(primitiveToWorld).transpose();
  data[0] = *reinterpret_cast<const Vector4 *>(&localToWorld[0][0]);
  data[1] = *reinterpret_cast<const Vector4 *>(&localToWorld[1][0]);
  data[2] = *reinterpret_cast<const Vector4 *>(&localToWorld[2][0]);
  data[3] = *reinterpret_cast<const Vector4 *>(&localToWorld[3][0]);

  const auto& worldToLocal = localToWorld.inverse().transpose();
  data[4] = *reinterpret_cast<const Vector4 *>(&worldToLocal[0][0]);
  data[5] = *reinterpret_cast<const Vector4 *>(&worldToLocal[1][0]);
  data[6] = *reinterpret_cast<const Vector4 *>(&worldToLocal[2][0]);
  data[7] = *reinterpret_cast<const Vector4 *>(&worldToLocal[3][0]);

  const auto &bounds =
      primitiveSceneProxy->getInstanceLocalBounds(instanceIndex);
  data[8] = Vector4{bounds.getCenter(), static_cast<float>(primitiveId)};
  data[9] = Vector4{bounds.getExtent(), 0.f};
}
}