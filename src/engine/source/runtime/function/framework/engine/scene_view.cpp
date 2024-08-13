#include "scene_view.h"

#include <string>

#include "core/math/matrix4.h"
#include "core/misc/config_cache_ini.h"
#include "function/framework/engine/scene_types.h"
#include "function/framework/world/world.h"
#include "resource/resource_path.h"


namespace GEngine {

ViewMatrices::ViewMatrices(const SceneViewInitOptions &initOptions) {
  viewMatrix = Matrix4x4::getTrans(-initOptions.viewOrigin) *
               initOptions.viewRotationMatrix;

  // For precision reasons the view matrix inverse is calculated independently.
  // Inv(RotationMatrix) = Transpose(RotationMatrix)
  invViewMatrix = initOptions.viewRotationMatrix.transpose() *
                  Matrix4x4::getTrans(initOptions.viewOrigin);

  projectionMatrix = initOptions.projectionMatrix;
  invProjectionMatrix = projectionMatrix.inverse();

  projectionViewMatrix = projectionMatrix * viewMatrix;

  // For precision reasons the view matrix inverse is calculated independently.
  invProjectionViewMatrix = invViewMatrix * invProjectionMatrix;
}

SceneViewFamily::SceneViewFamily(std::shared_ptr<World> &world)
    :frameNumber(0), m_scene(world->getScene()) {}

SceneView::SceneView(const SceneViewInitOptions &initOptions)
    : family(initOptions.viewFamily),
      viewActor(initOptions.viewActor),
      constrainedViewRect(initOptions.constrainedViewRect),
      unconstrainedViewRect(initOptions.constrainedViewRect),
      viewMatrices(initOptions),
      viewLocation(initOptions.viewLocation),
      viewRotation(initOptions.viewRotation),
      backgroundColor(initOptions.backgroundColor),
      fovx(initOptions.fovx) {
  // setup antialiasing method
  std::string antialiasing = "FXAA";
  ConfigCacheIni::instance().getString("Render", "antialiasing",
                                       ResourcePath::gameIni, antialiasing);
  if (antialiasing == "FXAA") {
    antiAliasingMethod = AntiAliasingMethod::FXAA;
  } else if (antialiasing == "TAA") {
    antiAliasingMethod = AntiAliasingMethod::TAA;
  } else if (antialiasing == "MSAA") {
    antiAliasingMethod = AntiAliasingMethod::MSAA;
  } else if (antialiasing == "TSR") {
    antiAliasingMethod = AntiAliasingMethod::TSR;
  }

}

}  // namespace GEngine