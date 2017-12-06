
#include "TPWAFunctionGPU.h"

TPWAFunctionGPU::TPWAFunctionGPU(string ConfigFileName) {
  Init(ConfigFileName);
}

void TPWAFunctionGPU::PrintMem(unsigned _nev) {
  TCache::PrintMem(mem_alloc, resonances.N(), _nev);
  TCache::PrintMem(mem_alloc_gpu, resonances.N(), _nev, "GPU");
}

TCalcCache * TPWAFunctionGPU::CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize) {
  TCalcCache * tmp = new TCalcGPU(x, offset, blockSize);
  mem_alloc_gpu += tmp->mem_alloc;
  return tmp;
}

void TPWAFunctionGPU::NCaches() {
  n_caches = 1;
  omp_set_num_threads(n_caches);
}
