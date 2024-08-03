#pragma once

#include <cassert>
#include <cstdint>
namespace GEngine {

template <typename ElementType, int NumElements>
class alignas(alignof(ElementType)) StaticArray {
public:
  ElementType &operator[](uint32_t index) {
    assert(index < NumElements);
    return m_data[index];
  }

  const ElementType &operator[](uint32_t index) const {
    assert(index < NumElements);
    return m_data[index];
  }

  uint32_t size() { return NumElements; }

  ElementType* data() { return m_data; }

private:
  ElementType m_data[NumElements];
};

}