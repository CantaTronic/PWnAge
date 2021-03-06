
#include "TCache.h"
#include "TResonanceSet.h"
// #include "TReaction.h"
#include <iostream>
using std::cerr;
using std::endl;
#include <cstring>

// TReaction * gReaction = 0;
int TCache::verbosity = 0;

TCache::TCache()
: memHandler(CPUMemoryHandler::Get()), mem_alloc(0), resonances(0),
  nev(0), v1kT2(3), s_kchan(3), precalc_factor(0) { }

TCache::TCache(TResonanceSet * r)
: memHandler(CPUMemoryHandler::Get()), mem_alloc(0), resonances(r),
  nev(0), v1kT2(3), s_kchan(3), precalc_factor(r->L()*3*4) { }

TCache::TCache(const TCache & x)
: memHandler(x.memHandler), mem_alloc(0), resonances(x.resonances),
  nev(x.nev), v1kT2(x.v1kT2), s_kchan(x.s_kchan),
  weight(x.weight), precalc_factor(x.precalc_factor) { }

// TCache & TCache::operator= (const TCache & x) {
//   if(this == &x) return *this;
//   resonances = x.resonances;
//   nev = x.nev;
//   return *this;
// }

void TCache::Cleanup() {
  v1kT2.Cleanup();
  s_kchan.Cleanup();
  weight.Cleanup();
  precalc_factor.Cleanup();
}

void TCache::Shift(unsigned offset) {
  v1kT2.Shift(offset, nev);
  s_kchan.Shift(offset, nev);
  weight.Shift(offset, nev);
  precalc_factor.Shift(offset, nev);
}

void TCache::PrintMem(unsigned & nbytes /*cache size in bytes*/, unsigned n_res/*number of resonances*/, unsigned n_ev/*number of events*/, const char * dev/*device*/) {
  /*TODO: write description here */
  if(verbosity >= 0) {
    string nm[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"};  //memory size units. TODO: rewrite it into more safe types
    double nbytesd = nbytes;
    unsigned step = 0;
    
    while(nbytesd >= 1024) { nbytesd /= 1024; step++; }
    cerr<<"Allocated "<<nbytesd<<" "<<nm[step].data()<<" of memory"<<endl;
    if(dev) cerr<<" on "<< dev<<endl;
    if(n_res || n_ev) cerr<<" for"<<endl;
    if(n_res) cerr<< n_res<<" resonances"<<endl;
    if(n_res && n_ev) cerr<<" and"<<endl;
    if(n_res) cerr<<n_ev<<" events"<<endl;
    cerr<<"\n"<<endl;
    cerr<<std::flush;
  }
  nbytes = 0;
}

CPUMemoryHandler * CPUMemoryHandler::Get() {
  static CPUMemoryHandler gInstance;
  return &gInstance;
}

void * CPUMemoryHandler::Allocate(unsigned n_bytes) {
  return malloc(n_bytes);
}

void CPUMemoryHandler::Free(void * p) {
  free(p);
}

void CPUMemoryHandler::Zero(void * p, unsigned n_bytes) {
  memset(p, 0, n_bytes);
}

void CPUMemoryHandler::Copy(void * to, const void * from, unsigned n_bytes) {
  memcpy(to, from, n_bytes);
}

