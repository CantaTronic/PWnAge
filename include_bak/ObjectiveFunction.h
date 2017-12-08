
#pragma once

#include "TFloat.h"

class ObjectiveFunction {
protected:
  TFloat last_val;
public:
  ObjectiveFunction(): last_val(0.) { }
  unsigned fcn_call;
/*  virtual void Eval(double * par, double & val, double * grad = 0, double * Z = 0) = 0;
  virtual TFloat LastVal() { return last_val; }
  virtual ~ObjectiveFunction() { }
  virtual unsigned NPar() = 0;
  virtual const char * Name(unsigned i) = 0;
  virtual TFloat Val(unsigned i) = 0;
  virtual TFloat Step(unsigned i) = 0;
  virtual TFloat Min(unsigned i) = 0;
  virtual TFloat Max(unsigned i) = 0;
  virtual bool IsFixed(unsigned i) = 0;*/
};
