#include "core/reflection/reflection.h"
#include "function/framework/actor/actor.h"
#include "function/framework/component/mesh/static_mesh_component.h"

namespace GEngine {

CLASS(MeshActor) : public Actor {
  REFLECTION_BODY(MeshActor)

public:
  MeshActor();

private:
  META_FIELD()
  std::shared_ptr<StaticMeshComponent> m_transformComponent;
};

}