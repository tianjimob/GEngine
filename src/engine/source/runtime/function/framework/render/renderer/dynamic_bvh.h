#pragma once

#include "core/math/bounds.h"
#include "core/math/vector4.h"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <vector>

namespace GEngine {

static constexpr uint32_t constLog2(uint32_t x) {
  return (x < 2) ? 0 : 1 + constLog2(x / 2);
}

template <uint32_t MaxChildren> class DynamicBVH {
public:
  void add(const Bounds &bounds, uint32_t index);
  bool check();

 private:
   static constexpr uint32_t IndexShift = constLog2(MaxChildren);
   static constexpr uint32_t ChildMask = MaxChildren - 1;
   
  struct Node {
    /* Index: low bits index child, high bits index node */

    // parent node index
    uint32_t parentIndex;

    // child num
    uint32_t numChildren;

    // if childIsLeaf[i] is true, m_leaves[childIndices[i] >> IndexShift] = currNodeIndex
    uint32_t childIndices[MaxChildren];
    bool childIsLeaf[MaxChildren];

    Bounds childBounds[MaxChildren];

    bool isFull() const { return numChildren == MaxChildren; }
    bool isRoot() const { return parentIndex == ~0u; }
    uint32_t getChildIndex(uint32_t nodeIndex) {
      return childIndices[nodeIndex & ChildMask];
    }
 };
 uint32_t m_root = ~0u;
 Bounds m_rootBounds;
 std::vector<Node> m_nodes;
 std::vector<uint32_t> m_leaves;

private:
  uint32_t allocNode();
  Node &getNode(uint32_t index) { return m_nodes[index >> IndexShift]; }
  // pair.first bestIndex, pair.second isLeaf
  std::pair<uint32_t, bool> findBestIndex(const Bounds &bounds);

  float costSurfArea(const Bounds &bounds) {
    Vector4 extent = bounds.max - bounds.min;
    return extent.x * extent.y + extent.x * extent.z + extent.y * extent.z;
  };
  bool checkNode(uint32_t index);
};

template <uint32_t MaxChildren>
void DynamicBVH<MaxChildren>::add(const Bounds &bounds, uint32_t index) {
  // leaves are not enough
  if (index >= m_leaves.size()) {
    m_leaves.reserve(m_leaves.size() * 2);
  }

  // allocate root node
  if (m_root == ~0u) {
    m_root = allocNode();
    getNode(m_root).parentIndex = ~0u;
    getNode(m_root).numChildren = 0;
  }

  auto &rootNode = getNode(m_root);

  // if root node is not full, add bounds as child of root node
  if (!rootNode.isFull()) {
    uint32_t nodeIndex = m_root | rootNode.numChildren;
    rootNode.childIndices[rootNode.numChildren] = index;
    rootNode.childBounds[rootNode.numChildren] = bounds;
    rootNode.childIsLeaf[rootNode.numChildren] = true;
    ++rootNode.numChildren;
    m_rootBounds += bounds;
    m_leaves.resize(index + 1, ~0u);
    m_leaves[index] = nodeIndex;

    checkNode(m_root);
    return;
  }

  // leave
  // not leave, not full
  // not leave, full
  auto [bestIndex, isLeaf] = findBestIndex(bounds);
  uint32_t lastIndex = bestIndex;
  bool addNewNode = isLeaf;
  if (!addNewNode && !getNode(bestIndex).isFull()) {
    bestIndex = bestIndex & ~ChildMask;
    addNewNode = false;
  } else if (!addNewNode && getNode(bestIndex).isFull()) {
    bestIndex = getNode(bestIndex).parentIndex;
    addNewNode = true;
  }
  uint32_t lastChildIndex = ~0u;
  if (addNewNode) {
    uint32_t newNodeIndex = allocNode();
    auto &newNode = getNode(newNodeIndex);
    newNode.numChildren = 1;
    newNode.childBounds[0] =
        getNode(bestIndex).childBounds[bestIndex & ChildMask];
    newNode.childIndices[0] =
        getNode(bestIndex).childIndices[bestIndex & ChildMask];
    lastChildIndex = newNode.childIndices[0];
    newNode.parentIndex = bestIndex;
    newNode.childIsLeaf[0] =
        getNode(bestIndex).childIsLeaf[bestIndex & ChildMask];
    if (newNode.childIsLeaf[0])
      m_leaves[newNode.childIndices[0]] = newNodeIndex;
    else
      getNode(newNode.childIndices[0]).parentIndex = newNodeIndex;

    getNode(bestIndex).childIndices[bestIndex & ChildMask] = newNodeIndex | (bestIndex & ChildMask);
    getNode(bestIndex).childIsLeaf[bestIndex & ChildMask] = false;

    bestIndex = newNodeIndex;

    checkNode(newNodeIndex);
    if (!newNode.childIsLeaf[0]) {
      checkNode(newNode.childIndices[0]);
    }
  }

  // add bounds node
  auto &children = getNode(bestIndex);
  children.childBounds[children.numChildren] = bounds;
  children.childIsLeaf[children.numChildren] = true;
  children.childIndices[children.numChildren] = index;
  m_leaves.resize(index + 1, ~0u);
  m_leaves[index] = (bestIndex & ~ChildMask) | children.numChildren;
  ++children.numChildren;
  checkNode(bestIndex);

  // propagate up to tree
  Bounds pathBounds = bounds;
  uint32_t pathIndex = getNode(bestIndex).parentIndex;
  while (pathIndex != ~0u) {
    auto &pathNode = getNode(pathIndex);
    getNode(pathIndex).childBounds[pathIndex & ChildMask] += pathBounds;

    // rotate, swap child and sibling if sibling has less cost
    {
      auto &node = getNode(pathIndex);

      if (!node.isRoot()) {
        Bounds excludeBounds;
        for (int i = 0; i < node.numChildren; ++i) {
          if (i != (pathIndex & ChildMask)) {
            excludeBounds += node.childBounds[i];
          }
        }

        auto &parentNode = getNode(node.parentIndex);

        float bestCost =
            costSurfArea(parentNode.childBounds[node.parentIndex & ChildMask]);

        uint32_t bestIndex = ~0u;
        
        for (int i = 0; i < parentNode.numChildren; ++i) {
          if (i != (node.parentIndex & ChildMask)) {
            float cost =
                costSurfArea(parentNode.childBounds[i] + excludeBounds);
            if (cost < bestCost) {
              bestCost = cost;
              bestIndex = (node.parentIndex & ~ChildMask) | i;
            }
          }
        }

        if (bestIndex != ~0u) {
          // swap pathIndex and bestIndex
          Bounds tempBounds = node.childBounds[pathIndex & ChildMask];
          uint32_t tempIndex = node.childIndices[pathIndex & ChildMask];
          bool tempIsLeaf = node.childIsLeaf[pathIndex & ChildMask];

          node.childBounds[pathIndex & ChildMask] =
              getNode(bestIndex).childBounds[bestIndex & ChildMask];
          node.childIndices[pathIndex & ChildMask] =
              getNode(bestIndex).childIndices[bestIndex & ChildMask];
          node.childIsLeaf[pathIndex & ChildMask] =
              getNode(bestIndex).childIsLeaf[bestIndex & ChildMask];
          if (node.childIsLeaf[pathIndex & ChildMask]) {
            m_leaves[node.childIndices[pathIndex & ChildMask]] = pathIndex;
          } else {
            getNode(node.childIndices[pathIndex & ChildMask]).parentIndex = pathIndex;
          }

          parentNode.childBounds[bestIndex & ChildMask] = tempBounds;
          parentNode.childIndices[bestIndex & ChildMask] = tempIndex;
          parentNode.childIsLeaf[bestIndex & ChildMask] = tempIsLeaf;
          if (tempIsLeaf) {
            m_leaves[tempIndex] = bestIndex;
          } else {
            getNode(tempIndex).parentIndex = bestIndex;
          }

          checkNode(pathIndex);
          checkNode(bestIndex);
        }
      }
    }

    pathBounds = pathNode.childBounds[pathIndex & ChildMask];
    pathIndex = pathNode.parentIndex;
  }
  m_rootBounds += pathBounds;
}

template <uint32_t MaxChildren> bool DynamicBVH<MaxChildren>::check() {
  for (int i = 0; i < m_nodes.size(); ++i) {
    for (int j = 0; j < m_nodes[i].numChildren; ++j) {
      auto check = checkNode((i << IndexShift) | j);
      if(!check) return false;
    }
  }
  return true;
}

template <uint32_t MaxChildren>
bool DynamicBVH<MaxChildren>::checkNode(uint32_t nodeIndex) {
  const auto &node = getNode(nodeIndex);

  if (!((nodeIndex & ChildMask) < node.numChildren)) {
    __debugbreak();
    return false;
  }

  if (!node.isRoot()) {
    if (!(getNode(node.parentIndex)
               .childIndices[node.parentIndex & ChildMask] &
           ~ChildMask) == (nodeIndex & ~ChildMask)) {
      __debugbreak();
      return false;
    }
  }

  uint32_t childIndex = node.childIndices[nodeIndex & ChildMask];
  if (node.childIsLeaf[nodeIndex & ChildMask]) {
    if (!(m_leaves[childIndex] == nodeIndex)) {
      __debugbreak();
      return false;
    }
  } else {
    const auto &children = getNode(childIndex);
    if (!(children.parentIndex == nodeIndex)) {
      __debugbreak();
      return false;
    }
  }

  return true;
}

template <uint32_t MaxChildren> uint32_t DynamicBVH<MaxChildren>::allocNode() {
  uint32_t allocIndex = m_nodes.size();
  if ((allocIndex + 1) > m_nodes.capacity()) {
    m_leaves.reserve(allocIndex + 1);
  }
  m_nodes.emplace_back();
  return allocIndex << IndexShift;
}

template <uint32_t MaxChildren>
std::pair<uint32_t, bool> DynamicBVH<MaxChildren>::findBestIndex(const Bounds &bounds) {
  uint32_t nodeIndex = m_root;

  float bestCost = FLT_MAX;
  uint32_t bestIndex = 0;
  float inducedCost = 0.f;

  bool isLeaf = false;
  do {
    auto &node = getNode(nodeIndex);
    if (node.isFull()) {
      float bestChildDist = FLT_MAX;
      uint32_t bestChildIndex = 0;

      for (int i = 0; i < node.numChildren; ++i) {
        Bounds& nodeBounds = node.childBounds[i];
        Vector4 delta =
            (bounds.min - nodeBounds.min) + (bounds.max - nodeBounds.max);
        float dist = std::abs(delta.x) + std::abs(delta.y) + std::abs(delta.z);
        if (dist < bestChildDist) {
          bestChildDist = dist;
          bestChildIndex = i;
        }
      }

      Bounds &bestChildBounds = node.childBounds[bestChildIndex];
      float totalCost = inducedCost + costSurfArea(bounds + bestChildBounds);
      float childCost = totalCost - costSurfArea(bestChildBounds);

      if (childCost >= bestCost) // not leave, full / not full
        break;

      if (totalCost < bestCost) {
        bestCost = totalCost;
        bestIndex = (nodeIndex & ~ChildMask) | bestChildIndex;
      }

      // leave
      if (node.childIsLeaf[bestChildIndex]) {
        isLeaf = true;
        break;
      }

      inducedCost = childCost;
      nodeIndex = node.childIndices[bestChildIndex];

    } else {
      return {node.parentIndex, false}; // not leave, not full
    }
  } while (nodeIndex != ~0u);

  return {bestIndex, isLeaf};
}
}