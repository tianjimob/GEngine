#include "primitive_scene_info.h"

namespace GEngine {

PrimitiveSceneInfo::PrimitiveSceneInfo(PrimitiveComponent &component,
                                       std::shared_ptr<Scene> scene)
    : m_proxy(component.getPrimitiveSceneProxy()),
      m_scene(scene),
      m_primitiveComponentId(component.getId()),
      m_instanceSceneDataOffset(-1),
      m_numInstanceSceneDataEntries(0) {}
}