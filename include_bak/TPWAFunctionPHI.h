
#pragma once

#include "TPWAFunction.h"
#include "TCalcPHI.h"

#ifdef __INTEL_COMPILER
class TPWAFunctionPHI: public TPWAFunction {
public:
  TPWAFunctionPHI(string ConfigFileName);
  ~TPWAFunctionPHI();
protected:
  unsigned mem_alloc_gpu;
  virtual void NCaches();
  virtual TCalcCache * CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize);
  virtual void PrintMem(unsigned _nev);
  virtual void DestroyCalcCache(TCalcCache* & x);
  virtual void Norm(double * par);
  virtual void Eval(double * par, double * grad, double * Z);
};
#endif
