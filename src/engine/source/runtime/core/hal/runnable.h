#pragma once

#include <cstdint>

namespace GEngine {

class Runnable {
public:
  virtual bool init() { return true; }
  virtual uint32_t run() = 0;
  virtual void stop() {}
  virtual void exit() {}
  virtual ~Runnable() {}
};

}