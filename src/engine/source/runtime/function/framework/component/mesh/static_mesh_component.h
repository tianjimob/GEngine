#pragma once

#include "core/math/transform.h"
#include "core/reflection/reflection.h"
#include "function/framework/component/mesh/static_mesh.h"
#include "function/framework/component/primitive/primitive_component.h"
#include <memory>
#include <string>
namespace GEngine {

CLASS(StaticMeshComponent) : public PrimitiveComponent {
  REFLECTION_BODY(StaticMeshComponent)
  
public:
  virtual std::shared_ptr<PrimitiveSceneProxy> createSceneProxy() override;

  std::shared_ptr<StaticMesh>& getStaticMesh() { return m_staticMesh; }

  virtual void postLoad(std::weak_ptr<GObject> parentObject) override; 

protected:
  META_FIELD()
  std::string m_resourceName;

  std::shared_ptr<StaticMesh> m_staticMesh;
};

}