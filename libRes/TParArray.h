
#pragma once

#include "TParType.h"
#include "OffloadDirectives.h"  //for offload directives: PWA_START_OFFLOAD, PWA_END_OFFLOAD
PWA_START_OFFLOAD

#include <vector>
#include <string>
#include "TFloat.h"

using std::vector;

struct TParArray {
  /*Структура массивов*/
  vector<std::string> names;
  vector<TFloat> parameters;
  vector<TParType> types;
  vector<TFloat> minima;
  vector<TFloat> maxima;
  vector<TFloat> steps;
  vector<unsigned> quantumNumbers;
  vector<TFloat> symmetryMultipliers;
  unsigned lmax;
  unsigned n_free_par;
  TParArray(): lmax(0), n_free_par(0) { }
 /* virtual ~TParArray() { }
  virtual void UpdateL();
  virtual void UpdateNFreePar();
  virtual void SetParameter(unsigned i, TFloat v);*/
};

PWA_END_OFFLOAD
