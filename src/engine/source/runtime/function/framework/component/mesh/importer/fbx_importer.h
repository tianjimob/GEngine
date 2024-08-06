#include "function/framework/component/mesh/static_mesh.h"
#include "ufbx/ufbx.h"
#include <memory>

namespace GEngine {

class FbxImporter {
public:
  static std::shared_ptr<StaticMesh> importStaticMesh(ufbx_node_list& nodes);
};

}