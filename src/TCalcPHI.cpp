
#ifdef __INTEL_COMPILER
#include "TCalcPHI.h"
#include "TCacheFile.h"
#include "cstring"
using std::cerr;
using std::endl;

template<class T>
void ToPhi(TPtr<T> & p, unsigned & mem_alloc) {
  if(!p) return;
  p.SetMemHandler(PHIMemoryHandler::Get());
  mem_alloc += p.AllocateCopy();
  PHIMemoryHandler::Get()->ToPHI(p.o, p.p, p.n*sizeof(T));
  p.o = 0;
}

template<class T>
void ToPhi(TPtrVec<T> & v, unsigned & mem_alloc) {
  for(unsigned i = 0; i < v.size(); i++)
    ToPhi(v[i], mem_alloc);
}

TCachePHI::TCachePHI(TCache * x, unsigned offset, unsigned _nev)
: TCache(*x) {
  nev = _nev;
  if(_nev == 0) nev = x->NEv() - offset;
  if(_nev > x->NEv() - offset) {
    cerr<<" Warning: TJpsiToKKpi0Cache("<<x<<", "<<offset<<", "<<_nev<<"):"<<endl
        <<"  _nev == "<<_nev<<" > "<<x<<"->nev - offset == "<<x->NEv()<<" - "<<offset<<endl;
    nev = x->NEv() - offset;
  }
  TCache::Shift(offset);
  ToPhi(v1kT2, mem_alloc);
  ToPhi(s_kchan, mem_alloc);
  ToPhi(weight, mem_alloc);
  ToPhi(precalc_factor, mem_alloc);
  {
    OCacheFile ocf(&buffer);
    ocf.Write(resonances);
    ocf.Write(nev);
    for(unsigned i = 0; i < v1kT2.size(); i++)
      ocf.Write(v1kT2[i].p);
    for(unsigned i = 0; i < s_kchan.size(); i++)
      ocf.Write(s_kchan[i].p);
    ocf.Write(weight.p);
    for(unsigned i = 0; i < precalc_factor.size(); i++)
      ocf.Write(precalc_factor[i].p);
  }
}

TCalcPHI::TCalcPHI(unsigned sz, const char * buf): TCalcCPU() {
  string s(buf, sz);
  ICacheFile icf(&s);
  resInst = new TResonanceSet;
  resonances = resInst;
  icf.Read(resInst);
  icf.Read(nev);
  precalc_factor.resize(resInst->L()*3*4);
  for(unsigned i = 0; i < v1kT2.size(); i++) {
    icf.Read(v1kT2[i].p);
    v1kT2[i].n = nev;
  }
  for(unsigned i = 0; i < s_kchan.size(); i++) {
    icf.Read(s_kchan[i].p);
    s_kchan[i].n = nev;
  }
  icf.Read(weight.p);
  weight.n = nev;
  for(unsigned i = 0; i < precalc_factor.size(); i++) {
    icf.Read(precalc_factor[i].p);
    precalc_factor[i].n = nev;
  }
  for(size_t i = 0; i < nev; i++)
    sum_w += weight[i];
  bw.resize(resInst->N()*3);
  blw_dec.resize(resInst->N()*3);
  ampl.resize(resInst->N()*4);
  Allocate();
  resInst->Changed();
  Recalc();
}

TCalcPHI::~TCalcPHI() {
  TCache::Cleanup();
}

PHIMemoryHandler * PHIMemoryHandler::Get() {
  static PHIMemoryHandler gInstance;
  return &gInstance;
}

void * PHIMemoryHandler::Allocate(unsigned n_bytes) {
  void * p;
  #pragma offload target(mic:0) in(n_bytes) out(p)
  {
    p = malloc(n_bytes);
  }
  return p;
}

void PHIMemoryHandler::Free(void * p) {
  #pragma offload target(mic:0) in(p)
  {
    free(p);
  }
}

void PHIMemoryHandler::Zero(void * p, unsigned n_bytes) {
  #pragma offload target(mic:0) in(p, n_bytes)
  {
    memset(p, 0, n_bytes);
  }
}

void PHIMemoryHandler::Copy(void * to, const void * from, unsigned n_bytes) {
  #pragma offload target(mic:0) in(to, from, n_bytes)
  {
    memcpy(to, from, n_bytes);
  }
}

void PHIMemoryHandler::ToPHI(void * p, void * p_d, unsigned n_bytes) {
  // not the most effective way, but safe
  char * _p = reinterpret_cast<char*>(p);
  #pragma offload target(mic:0) in(_p:length(n_bytes)) in(p_d)
  {
    memcpy(p_d, _p, n_bytes);
  }
}
/*
void PHIMemoryHandler::FromPHI(void * p, unsigned nbytes) {
  // not the most effective way, but safe
  char * _p = reinterpret_cast<char*>(p);
  #pragma offload target(mic:0) out(_p: length(nbytes)) in(p_d)
  {
    memcpy(_p, p_d, n_bytes);
  }
}
*/
#endif
