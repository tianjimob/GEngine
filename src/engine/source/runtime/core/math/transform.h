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
        META_FIELD()
        Vector3 position{Vector3::ZERO};

        META_FIELD()
        Vector3    scale {Vector3::UNIT_SCALE};

        META_FIELD()
        Quaternion rotation {Quaternion::IDENTITY};

        Transform() = default;
        Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) :
            position {position}, scale {scale}, rotation {rotation}
        {}

        Matrix4x4 getMatrix() const
        {
            Matrix4x4 temp;
            temp.makeTransform(position, scale, rotation);
            return temp;
        }
    };
} // namespace Piccolo
