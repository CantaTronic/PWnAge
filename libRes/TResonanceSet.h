
#pragma once

#include "TParType.h"
#include "TParArray.h"
#include "TFitPar.h"
#include "TFloat.h"   //changing precision double/float
#include "OffloadDirectives.h"  //for offload directives: PWA_START_OFFLOAD, PWA_END_OFFLOAD


PWA_START_OFFLOAD
#include <vector>
#include <string>
PWA_END_OFFLOAD
using std::vector;
using std::string;

PWA_START_OFFLOAD

class TResonance {
public:
  static const unsigned nPar, nQN;
protected:
  TParArray * par;
  unsigned offsetPar, offsetQN, offsetSM;
public:
  TResonance()
  : par(0), offsetPar(0), offsetQN(0), offsetSM(0) { }
  operator bool() { return par; }
  TResonance(TParArray * _par, unsigned _num)
    : par(_par), offsetPar(_num*nPar), offsetQN(_num*nQN), offsetSM(_num*3) { }
  TResonance(const TResonance & x)
    : par(x.par), offsetPar(x.offsetPar), offsetQN(x.offsetQN), offsetSM(x.offsetSM) { }
  TResonance & operator= (const TResonance & x);
  // add info on reactions [kinematic channels] it contributes to
  // need to have a list of globally defined reaction and channels?
  // additionally introduce decays maechanisms (--> modes + couplings)
  TFitPar Par(unsigned i);
  TFitPar _M() { return Par(0); }
  TFitPar _G() { return Par(1); }
  TFitPar _r() { return Par(2); }
  TFitPar _Coupling() { return Par(3); }
  TFitPar _Phase() { return Par(4); }
  TFloat M() { return par->parameters[offsetPar]; }
  TFloat G() { return par->parameters[offsetPar+1]; }
  TFloat r() { return par->parameters[offsetPar+2]; }
  TFloat Coupling() { return par->parameters[offsetPar+3]; }
  TFloat Phase() { return par->parameters[offsetPar+4]; }
  unsigned J() { return par->quantumNumbers[offsetQN]; }
  unsigned P() { return par->quantumNumbers[offsetQN+1]; } // +1, -1;
  unsigned C() { return par->quantumNumbers[offsetQN+2]; } // +1, -1;
  unsigned type() { return par->quantumNumbers[offsetQN+3]; } // mass dependent, not mass dependent
  void J(unsigned v) { par->quantumNumbers[offsetQN] = v; par->UpdateL(); }
  void P(unsigned v) { par->quantumNumbers[offsetQN+1] = v; }
  void C(unsigned v) { par->quantumNumbers[offsetQN+2] = v; }
  void type(unsigned v) { par->quantumNumbers[offsetQN+3] = v; }
  TFloat & SymmetryMultiplier(unsigned i) { return par->symmetryMultipliers[offsetSM+i]; }
  // need to have channel specif data
  // productionCouplings[jpsi_decay_channl][kinematic_channel]
};

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

std::ostream & operator<< (std::ostream & os, TFitPar _p);

PWA_END_OFFLOAD
