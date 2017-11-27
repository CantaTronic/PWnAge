
#pragma once

#include "TPWAFunction.h"

class TPWAFunctionCPU: public TPWAFunction {
public:
  TPWAFunctionCPU(string /*ConfigFileName*/);/*
protected:
  virtual void NCaches();
  virtual TCalcCache * CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize);
  virtual void PrintMem(unsigned _nev);*/
};
