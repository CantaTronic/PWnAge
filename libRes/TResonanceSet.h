
#pragma once

#include "TParArray.h"
#include "TFitPar.h"
#include "OffloadDirectives.h"  //for offload directives: PWA_START_OFFLOAD, PWA_END_OFFLOAD
#include "TResonance.h"

PWA_START_OFFLOAD
#include <string>
PWA_END_OFFLOAD
using std::string;

PWA_START_OFFLOAD

class ICacheFile;
class OCacheFile;

class TResonanceSet: protected TParArray {
public:
  TResonanceSet(): TParArray() { }
  TResonanceSet(const TResonanceSet & x): TParArray(x) { }
  TResonanceSet & operator= (const TResonanceSet & x); // copy-assignment
  virtual ~TResonanceSet() { }
  unsigned L() { return lmax; }
  unsigned N() { return names.size(); }
  unsigned NPar() { return parameters.size(); }
  unsigned NFreePar() { return n_free_par; }
  unsigned NZ() { return n_free_par*(n_free_par+1)/2; }
  TResonance Add(string name);
  TResonance Res(unsigned i);
  TFitPar Par(unsigned i);
  void Clear();
  void SetParameters(double * par);
  void GetParameters(double * par);
  void Changed();
  void PrintParameters();
  void PrintChanged();
  void Write(OCacheFile * cf);
  void Read(ICacheFile * cf);
  bool operator== (const TResonanceSet & x);
  bool operator!= (const TResonanceSet & x);
};

PWA_END_OFFLOAD
