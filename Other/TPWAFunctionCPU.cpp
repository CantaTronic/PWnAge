
#include "TPWAFunctionCPU.h"
#include <string>
#include <omp.h>
#include <iostream>

using std::cout;
using std::endl;


void TPWAFunctionCPU::NCaches() {
  /*set up number of caches equal to number of CPU threads we have*/
  n_caches = omp_get_max_threads();
}

TCalcCache * TPWAFunctionCPU::CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize) {
  /*create and return specific cache for CPU calculations*/
  TCalcCache * tmp = new TCalcCPU(x, offset, blockSize);
  mem_alloc += tmp->mem_alloc;  //calculate how much memory do we use so far
  return tmp;   
}

void TPWAFunctionCPU::PrintMem(unsigned _nev) {
  TCache::PrintMem(mem_alloc, resonances.N(), _nev);
}

TPWAFunctionCPU::TPWAFunctionCPU(std::string ConfigFileName) {
  Init(ConfigFileName);
}
