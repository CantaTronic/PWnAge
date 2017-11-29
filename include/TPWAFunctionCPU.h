
#pragma once

#include "TPWAFunction.h"
#include "TCalcCPU.h"

class TPWAFunctionCPU: public TPWAFunction {
public:
  TPWAFunctionCPU(/*std::string*/);/*
protected:
  virtual void NCaches();
  virtual TCalcCache * CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize);
  virtual void PrintMem(unsigned _nev);*/
};
