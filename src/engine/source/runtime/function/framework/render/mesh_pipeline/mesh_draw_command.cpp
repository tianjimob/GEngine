#include "mesh_draw_command.h"
#include "function/framework/render/rhi/rhi.h"
#include "function/framework/render/rhi/rhi_resource.h"
#include <memory>

namespace GEngine {

void MeshDrawCommand::submitDraw(const void *parametersData) {
  std::shared_ptr<RHIGraphicsPipelineState> pipeline =
      GlobalRHI->createGraphicsPipelineState(pipelineState);

  GlobalRHI->getGraphicsContext()->RHISetGraphicsPipelineState(pipeline, parametersData);
}

}