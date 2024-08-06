#pragma once

#include "function/framework/component/mesh/static_mesh.h"
#include <memory>
#include <string>
#include <unordered_map>
namespace GEngine {

class StaticMeshManager {
public:
    static std::shared_ptr<StaticMesh> loadStaticMesh(const std::string& staticMeshPath);
private:
    static std::unordered_map<std::string, std::shared_ptr<StaticMesh>> m_staticMeshMap;
};

}