#pragma once

#include <memory>
#include <vector>
namespace GEngine {

class SceneView {};

// A set of views into a scene which only have different view transforms and owner actors.
class SceneViewFamily {
private:
  std::vector<std::shared_ptr<SceneView>> m_sceneViews;
};

}