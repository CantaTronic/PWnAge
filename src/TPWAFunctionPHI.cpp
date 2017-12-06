
#include "TPWAFunction.h"

#ifdef __INTEL_COMPILER
void TPWAFunctionPHI::NCaches() {
  unsigned t_n_caches;
  #pragma offload target(mic:0) out(t_n_caches)
  {
    t_n_caches = omp_get_max_threads();
  }TPWAFunction
}

TCalcCache * TPWAFunctionPHI::CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize) {
  TCachePHI tmpCachePHI(x, offset, blockSize);
  const char * buf = tmpCachePHI.Buffer();
  unsigned sz = tmpCachePHI.BufSize();
  void * tmpCalcPHI;
  #pragma offload target(mic:0) in(sz) in(buf:length(sz)) out(tmpCalcPHI)
  {
    tmpCalcPHI = new TCalcPHI(sz, buf);
  }
  return reinterpret_cast<TCalcCache*>(tmpCalcPHI);
}

void TPWAFunctionPHI::PrintMem(unsigned _nev) {
  cerr<<"TPWAFunctionPHI::PrintMem not implemented yet"<<endl;
}

void TPWAFunctionPHI::DestroyCalcCache(TCalcCache* & x) {
  void * tmp = x;
  #pragma offload target(mic:0) in(tmp)
  {
    auto * p = reinterpret_cast<TCalcPHI*>(tmp);
    delete p;
  }
  x = 0;
}

TPWAFunctionPHI::TPWAFunctionPHI(string ConfigFileName) {
  Init(ConfigFileName);
}
#endif
