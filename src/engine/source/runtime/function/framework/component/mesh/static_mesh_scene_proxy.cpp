#include "static_mesh_scene_proxy.h"

namespace GEngine {

StaticMeshSceneProxy::StaticMeshSceneProxy(StaticMeshComponent &component):m_staticMesh(component.getStaticMesh()) {
  
}

}