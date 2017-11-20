
#pragma once

#include "OffloadDirectives.h"
#include "TFloat.h"
#include "TCache.h"

PWA_START_OFFLOAD
class TJpsiToKKpi0Const: public TReaction {
public:
  virtual TFloat Dens(unsigned m) { return jpsiDensMatr[m]; }
  virtual TFloat Mass(unsigned k, unsigned iparticle) { return masses[k][iparticle]; }
  virtual TFloat rRes() { return r_jpsi; }
protected:
  static const TFloat jpsiDensMatr[4];
  static const TFloat r_jpsi;
  static const TFloat masses[3][2];
};
PWA_END_OFFLOAD
