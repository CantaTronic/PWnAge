
#pragma once

#include "OffloadDirectives.h"
#include "cfloat.h"

PWA_START_OFFLOAD

// TODO now hardcoded 3 channels!
class TReaction {
public:
  virtual TFloat Dens(unsigned m) = 0;
  virtual TFloat Mass(unsigned k, unsigned iparticle) = 0;
  virtual TFloat rRes() = 0;
};
extern TReaction * gReaction;

PWA_END_OFFLOAD
