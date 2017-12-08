
#pragma once

#include <string> //vt
using std::string;


#include "TResonanceSet.h"/**/
#include "TInputFiles.h"
#include "deviceType.h"
#include "ObjectiveFunction.h"/*
#include "Minimizer.h"

class TCache;
class TCalcCache;
class TDataCache;*/
// https://isocpp.org/wiki/faq/pointers-to-members
// class provides FCN, passes resonance parameters to a minimizer
class TPWAFunction: public ObjectiveFunction {
  /*
protected:
  unsigned n_thr;
  vector<TCalcCache*> calc_data, calc_mc;
  TDataCache * data, * mc;*/
  TInputFiles inputFiles;
  TResonanceSet resonances;
  /*
//   TDevice dev;
  unsigned n_caches;
  unsigned mem_alloc;
  TFloat norm;
  TFloat * deltaPar;
  TFloat * N1;
  virtual void NCaches() = 0;
  virtual TCalcCache * CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize) = 0;
  virtual void PrintMem(unsigned _nev) = 0;
  virtual void DestroyCalcCache(TCalcCache* & x);
  virtual void Norm(double * par);
  virtual void Eval(double * par, double * grad, double * Z);*/
public:
  static double grad_step;
  static int verbosity;
  std::string test (std::string str) {return "test "+str;}
  TPWAFunction(/*deviceType dev, string ConfigFileName*/);    
  static /*void*/ TPWAFunction * Create (/*std::string */deviceType dev, std::string ConfigFileName);
  void Init(/*deviceType dev, */string ConfigFileName);/*
  virtual ~TPWAFunction();
  virtual void SetParameters(double * par) { resonances.SetParameters(par); }
  virtual void GetParameters(double * par) { resonances.GetParameters(par); }
  virtual void Eval(double * par, double & val, double * grad = 0, double * Z = 0);
  virtual TFloat Fit(ROOTMinimizer::Strategy s = ROOTMinimizer::MIGRAD);
  virtual TFloat Test(unsigned n, double * _grad = 0);
  virtual TFloat TestGrad(unsigned n, unsigned nthr, double * _grad = 0);
  virtual TFloat TestGZ(unsigned n);
  virtual unsigned NPar() { return resonances.NPar(); }
  virtual unsigned NFreePar() { return resonances.NFreePar(); }
  virtual unsigned NZ() { return resonances.NZ(); }
  virtual const char * Name(unsigned i)
      { return resonances.Par(i).CName(); }
  virtual TFloat Val(unsigned i) { return resonances.Par(i); }
  virtual TFloat Step(unsigned i) { return resonances.Par(i).Step(); }
  virtual TFloat Min(unsigned i) { return resonances.Par(i).Min(); }
  virtual TFloat Max(unsigned i) { return resonances.Par(i).Max(); }
  virtual bool IsFixed(unsigned i) { return resonances.Par(i).IsFixed(); }*/
private:

};

