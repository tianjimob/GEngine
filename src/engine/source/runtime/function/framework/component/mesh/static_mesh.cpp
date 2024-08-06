#include "static_mesh.h"

#include "core/math/bounds.h"
#include "core/math/box_sphere_bounds.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "function/framework/component/mesh/vertex_streams.h"
#include "function/framework/render/rhi/rhi.h"
#include <algorithm>
#include <cmath>

namespace GEngine {

void StaticMeshModel::build() {
  m_positionBuffer = GlobalRHI->createVertexBuffer(
      m_meshDesciptor->m_vertexPositions.data(),
      m_meshDesciptor->m_vertexPositions.size() * sizeof(Vector3));

  {
    VertexStream stream;
    stream.vertexBuffer = m_positionBuffer;
    stream.stride = sizeof(Vector3);
    stream.offset = 0;
    m_vertexStreams.addVertexStream(stream);
  }

  if (!m_meshDesciptor->m_vertexNormals.empty()) {
    m_normalBuffer = GlobalRHI->createVertexBuffer(
        m_meshDesciptor->m_vertexNormals.data(),
        m_meshDesciptor->m_vertexNormals.size() * sizeof(Vector3));

    VertexStream stream;
    stream.vertexBuffer = m_normalBuffer;
    stream.stride = sizeof(Vector3);
    stream.offset = 0;
    m_vertexStreams.addVertexStream(stream);
  }

  if (!m_meshDesciptor->m_vertexTangents.empty()) {
    m_tangentBuffer = GlobalRHI->createVertexBuffer(
        m_meshDesciptor->m_vertexTangents.data(),
        m_meshDesciptor->m_vertexTangents.size() * sizeof(Vector3));

    VertexStream stream;
    stream.vertexBuffer = m_tangentBuffer;
    stream.stride = sizeof(Vector3);
    stream.offset = 0;
    m_vertexStreams.addVertexStream(stream);
  }

  if (!m_meshDesciptor->m_vertexColors.empty()) {
    m_colorBuffer = GlobalRHI->createVertexBuffer(
        m_meshDesciptor->m_vertexColors.data(),
        m_meshDesciptor->m_vertexColors.size() * sizeof(Vector4));

    VertexStream stream;
    stream.vertexBuffer = m_colorBuffer;
    stream.stride = sizeof(Vector4);
    stream.offset = 0;
    m_vertexStreams.addVertexStream(stream);
  }

  if (!m_meshDesciptor->m_vertexUVs.empty()) {
    m_uvBuffer = GlobalRHI->createVertexBuffer(
        m_meshDesciptor->m_vertexUVs.data(),
        m_meshDesciptor->m_vertexUVs.size() * sizeof(Vector2));

    VertexStream stream;
    stream.vertexBuffer = m_uvBuffer;
    stream.stride = sizeof(Vector2);
    stream.offset = 0;
    m_vertexStreams.addVertexStream(stream);
  }
}

BoxSphereBounds StaticMeshModel::computeBoundsFromPositionBuffer() {
  Bounds bound;
  for (auto &position : m_meshDesciptor->m_vertexPositions) {
    bound += position;
  }

  Vector3 center = bound.getCenter();
  float radius = 0.f;
  for (auto &position : m_meshDesciptor->m_vertexPositions) {
    radius = std::max(radius, (position - center).squaredLength());
  }

  return {center, bound.getExtent(), std::sqrt(radius)};
}

}  // namespace GEngine