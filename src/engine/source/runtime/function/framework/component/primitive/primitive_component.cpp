#include "primitive_component.h"
#include <atomic>

namespace GEngine {

std::atomic_uint32_t PrimitiveComponent::s_nextId{0};

PrimitiveComponent::PrimitiveComponent() : m_primitiveComponentId(s_nextId++) {}
}