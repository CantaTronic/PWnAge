
#pragma once

#include <string> //vt
using std::string;


#include "TResonanceSet.h"/**/
#include "TInputFiles.h"
#include "deviceType.h"
#include "ObjectiveFunction.h"
// #include "Minimizer.h"

class TCache;
class TCalcCache;
class TDataCache;
// https://isocpp.org/wiki/faq/pointers-to-members
// class provides FCN, passes resonance parameters to a minimizer
class TPWAFunction: public ObjectiveFunction {
  
protected:/*
  unsigned n_thr;*/
  vector<TCalcCache*> calc_data, calc_mc;   //actually calculated data in memory (TODO: rewrite into lists to safe time for big inputs?)
  TDataCache * data, * mc;    //pointers to caches with data
  TInputFiles inputFiles;
  TResonanceSet resonances;
  
//   TDevice dev;
  unsigned n_caches;  //number of caches we have (equal to number of GPU cards of CPU threads)
 unsigned mem_alloc;  //how much memory do we use so far
 /* 
  TFloat norm;
  TFloat * deltaPar;
  TFloat * N1;*/
  virtual void NCaches() = 0;
  virtual TCalcCache * CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize) = 0;
  virtual void PrintMem(unsigned _nev) = 0;
  virtual void DestroyCalcCache(TCalcCache* & x);
 /* virtual void Norm(double * par);
  virtual void Eval(double * par, double * grad, double * Z);*/
public:
  static double grad_step;
  static int verbosity;
  TPWAFunction(/*deviceType dev, string ConfigFileName*/);    
  static /*void*/ TPWAFunction * Create (deviceType dev, std::string ConfigFileName);
  void Init(string ConfigFileName);
  virtual ~TPWAFunction();
  virtual void SetParameters(double * par) { resonances.SetParameters(par); }
  virtual void GetParameters(double * par) { resonances.GetParameters(par); }/*
  virtual void Eval(double * par, double & val, double * grad = 0, double * Z = 0);*/
//   virtual TFloat Fit(ROOTMinimizer::Strategy s = ROOTMinimizer::MIGRAD);
  /*virtual TFloat Test(unsigned n, double * _grad = 0);
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

