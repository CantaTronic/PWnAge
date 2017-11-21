
#pragma once

#include "OffloadDirectives.h"
PWA_START_OFFLOAD
class MemoryHandler {
public:
  virtual ~MemoryHandler() { }
  virtual void * Allocate(unsigned n_bytes) = 0;
  virtual void Free(void * p) = 0;
  virtual void Zero(void * p, unsigned n_bytes) = 0;
  virtual void Copy(void * to, const void * from, unsigned n_bytes) = 0;
};
PWA_END_OFFLOAD
