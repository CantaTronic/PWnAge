
#include "TPWAFunctionCPU.h"
#include <string>

/*
void TPWAFunctionCPU::NCaches() {
  n_caches = omp_get_max_threads();
}

TCalcCache * TPWAFunctionCPU::CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize) {
  TCalcCache * tmp = new TCalcCPU(x, offset, blockSize);
  mem_alloc += tmp->mem_alloc;
  return tmp;
}

void TPWAFunctionCPU::PrintMem(unsigned _nev) {
  TCache::PrintMem(mem_alloc, resonances.N(), _nev);
}*/

TPWAFunctionCPU::TPWAFunctionCPU(std::string ConfigFileName) {
  Init(ConfigFileName);
}
