#pragma once

#include "core/hal/runnable.h"

namespace GEngine {

extern bool gUseRenderingThread;
extern thread_local bool gIsRenderingThread;

extern bool gUseRHIThread;
extern thread_local bool gIsRHIThread;

class RendringThread : public Runnable {
public:
  virtual bool init() override { gIsRenderingThread = true; return true;}
  virtual uint32_t run() override;
};

class RHIThread : public Runnable {
public:
  virtual bool init() override { gIsRHIThread = true; return true;}
  virtual uint32_t run() override;
};

void startRenderingThread();
}