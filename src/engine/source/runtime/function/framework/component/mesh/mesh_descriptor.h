#pragma once

#include <vector>

#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"

namespace GEngine {

class MeshDescriptor {
 public:
 private:
  friend class FbxImporter;
  friend class ObjImporter;
  friend class StaticMeshModel;

  std::vector<Vector3> m_vertexPositions;
  std::vector<Vector3> m_vertexNormals;
  std::vector<Vector3> m_vertexTangents;
  std::vector<Vector4> m_vertexColors;
  std::vector<Vector2> m_vertexUVs;
};

}  // namespace GEngine