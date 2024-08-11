#include "scene_view.h"
#include "core/math/matrix4.h"

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

SceneView::SceneView(const SceneViewInitOptions &initOptions)
    : family(initOptions.viewFamily), viewActor(initOptions.viewActor),
      constrainedViewRect(initOptions.constrainedViewRect),
      unconstrainedViewRect(initOptions.constrainedViewRect),
      viewMatrices(initOptions), viewLocation(initOptions.viewLocation),
      viewRotation(initOptions.viewRotation),
      backgroundColor(initOptions.backgroundColor), fovx(initOptions.fovx) {}

}  // namespace GEngine