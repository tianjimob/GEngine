#pragma once
#include "core/math/matrix4.h"
#include "core/math/quaternion.h"
#include "core/math/vector3.h"
#include "core/reflection/reflection.h"

namespace GEngine
{
    CLASS(Transform) : public GObject
    {
        REFLECTION_BODY(Transform)

    public:
        Vector3    m_position {Vector3::ZERO};
        Vector3    m_scale {Vector3::UNIT_SCALE};
        Quaternion m_rotation {Quaternion::IDENTITY};

        Transform() = default;
        Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) :
            m_position {position}, m_scale {scale}, m_rotation {rotation}
        {}

        Matrix4x4 getMatrix() const
        {
            Matrix4x4 temp;
            temp.makeTransform(m_position, m_scale, m_rotation);
            return temp;
        }
    };
} // namespace Piccolo
