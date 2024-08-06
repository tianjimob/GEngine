#include "function/framework/component/mesh/static_mesh.h"
#include "tinyobjloader/tiny_obj_loader.h"
#include <memory>
#include <vector>

namespace GEngine {

class ObjImporter {
public:
  static std::shared_ptr<StaticMesh> importStaticMesh(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);
};

}