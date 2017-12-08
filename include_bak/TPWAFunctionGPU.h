
#pragma once

#include "TPWAFunction.h"
#include "TCalcGPU.h"

class TPWAFunctionGPU: public TPWAFunction {
public:
  TPWAFunctionGPU(string ConfigFileName);
protected:
  unsigned mem_alloc_gpu;
  virtual void NCaches();
  virtual TCalcCache * CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize);
  virtual void PrintMem(unsigned _nev);
};
