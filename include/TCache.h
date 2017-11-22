
#pragma once

#include "OffloadDirectives.h"
#include "cfloat.h"
/*#include "TPtr.h"*/

class TResonanceSet;

PWA_START_OFFLOAD
// TODO now hardcoded 3 channels!
class TReaction {
public:
  virtual TFloat Dens(unsigned m) = 0;
  virtual TFloat Mass(unsigned k, unsigned iparticle) = 0;
  virtual TFloat rRes() = 0;
};
extern TReaction * gReaction;
/**
dimensions:
  r[0,nres) -- num. of resonances
  k[0,3) -- num. of channel
  i[0,nev) -- num. of event
  m[0,4) -- mu index
data:
  unsigned int nres
  size_t nev
  TFloat v1kT2[k][i] k,iev
  TFloat s_kchan[k][i] k,iev
  TFloat weight[i] iev
  cfloat precalc_factor[l*3*4+k*4+m][i] l,k,m,iev
  cfloat bw[r*3+k][i] r,k,iev
  TFloat blw_dec[r*3+k][i] r,k,iev
  TFloat var_factor[r*3+k][i] r,k,iev
  cfloat prod_factor[r*3+k] r,k
  cfloat ampl[r*4+m][i] r,m,iev
*/


class TCache {
public:/*
  TCache();
  TCache(TResonanceSet * r);
  TCache(const TCache & x);              // copy-constructor
  virtual ~TCache() { }
  virtual unsigned NEv() { return nev; }
  MemoryHandler * memHandler;
  static void PrintMem(unsigned & nbytes, unsigned n_res = 0,
                       unsigned n_ev = 0, const char * dev = 0);
  unsigned mem_alloc;*/
  static int verbosity;/*
protected:
  virtual void Allocate() = 0;
  virtual void Cleanup();
  virtual void Shift(unsigned offset);
  TResonanceSet * resonances;
  unsigned nev;
  TPtrVec<TFloat> v1kT2;
  TPtrVec<TFloat> s_kchan;
  TPtr<TFloat> weight;
  TPtrVec<cfloat> precalc_factor;
private:
  TCache & operator= (const TCache & x); // copy-assignment*/
};

PWA_END_OFFLOAD
