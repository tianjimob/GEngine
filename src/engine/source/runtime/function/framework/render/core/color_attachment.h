#pragma once

#include "function/framework/render/rhi/rhi_resource.h"
#include <memory>

namespace GEngine {

class ColorAttachment {
public:
  ColorAttachment() {}
  virtual ~ColorAttachment() {}

  const std::shared_ptr<RHIImage>& getColorAttachmentImage() const { return m_rhiImage; }

protected:
  std::shared_ptr<RHIImage> m_rhiImage;
};

}