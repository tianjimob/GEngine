#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <vector>

#include "core/math/matrix4.h"
#include "core/math/rect.h"
#include "core/math/rotator.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "function/framework/actor/actor.h"
#include "function/framework/engine/scene_types.h"

namespace GEngine {

class SceneViewFamily;
class Actor;

struct SceneViewInitOptions {
  std::shared_ptr<SceneViewFamily> viewFamily;
  std::shared_ptr<Actor> viewActor;

  Rect constrainedViewRect;
  Rect unconstrainedViewRect;

  // camera Inverse rotation matrix
  Matrix4x4 viewRotationMatrix;
  Matrix4x4 projectionMatrix;

  // camera position
  Vector3 viewOrigin;

  Vector3 viewLocation;
  Rotator viewRotation;
  float fovx;

  // linear clear color
  Vector4 backgroundColor;
};

struct ViewMatrices {
  ViewMatrices(const SceneViewInitOptions &initOptions);

  /*
    WorldToView

    The default camera is located at the origin of the world coordinate
    system,and the camera coordinate system is the same as the world
    coordinate,system, the camera is facing the -z axis and UP direction is the y
    axis

    Assuming that camera has C = RT transformation, the view matrix is the
    inverse transformation M = Inv(C) = Inv(T) * Inv(R)
  */
  Matrix4x4 viewMatrix;

  // ViewToWorld
  Matrix4x4 invViewMatrix;

  // ViewToClip, clip space z = 10 is the near plane and z = 1000 is far plane
  Matrix4x4 projectionMatrix;

  // ClipToView
  Matrix4x4 invProjectionMatrix;

  // WorldToClip
  Matrix4x4 projectionViewMatrix;

  // ClipToWorld
  Matrix4x4 invProjectionViewMatrix;
};

class SceneView {
 public:
  SceneView(const SceneViewInitOptions &initOptions);

  std::weak_ptr<SceneViewFamily> family;

  // The actor which is being viewed from.
  std::shared_ptr<Actor> viewActor;

  // Final position of the view in the final render target (in pixels),
  // potentially constrained by an aspect ratio requirement (black bars)
  const Rect constrainedViewRect;

  // Raw view size (in pixels), used for screen space calculations
  Rect unconstrainedViewRect;

  ViewMatrices viewMatrices;

  // Variables used to determine the view matrix
  Vector3 viewLocation;
  Rotator viewRotation;

  // linear clear color
  Vector4 backgroundColor;

  // Actual field of view
  float fovx;

  // The antialiasing method.
	AntiAliasingMethod AntiAliasingMethod;

  std::set<uint32_t> hiddenPrimitives;
};

// A set of views into a scene which only have different view transforms and
// owner actors.
class SceneViewFamily {
 public:
  void addSceneView(std::shared_ptr<SceneView> sceneView) {
    m_sceneViews.emplace_back(sceneView);
  }

 private:
  std::vector<std::shared_ptr<SceneView>> m_sceneViews;
};

}  // namespace GEngine