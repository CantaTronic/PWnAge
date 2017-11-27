
#include "TPWAFunction.h"
/*
#include "TDataCache.h"
// 
*/
#include "../libConfPars/ConfigParser.h"
#include <cstring>
#include <iostream>
using std::cerr;
using std::endl;
#include <omp.h>

int TPWAFunction::verbosity = 1;
double TPWAFunction::grad_step = 1e-5;
/*


// void TPWAFunctionGPU::NCaches() {
//   n_caches = 1;
//   omp_set_num_threads(n_caches);
// }
#ifdef __INTEL_COMPILER
void TPWAFunctionPHI::NCaches() {
  unsigned t_n_caches;
  #pragma offload target(mic:0) out(t_n_caches)
  {
    t_n_caches = omp_get_max_threads();
  }
  n_caches = t_n_caches;
}
#endif

// TCalcCache * TPWAFunctionGPU::CreateCalcCache(TCache * x, unsigned offset, unsigned blockSize) {
//   TCalcCache * tmp = new TCalcGPU(x, offset, blockSize);
//   mem_alloc_gpu += tmp->mem_alloc;
//   return tmp;
// }
#ifdef __INTEL_COMPILER
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
#endif


// void TPWAFunctionGPU::PrintMem(unsigned _nev) {
//   TCache::PrintMem(mem_alloc, resonances.N(), _nev);
//   TCache::PrintMem(mem_alloc_gpu, resonances.N(), _nev, "GPU");
// }
#ifdef __INTEL_COMPILER
void TPWAFunctionPHI::PrintMem(unsigned _nev) {
  cerr<<"TPWAFunctionPHI::PrintMem not implemented yet"<<endl;
}
#endif
void TPWAFunction::DestroyCalcCache(TCalcCache* & x) {
  delete x;
}
#ifdef __INTEL_COMPILER
void TPWAFunctionPHI::DestroyCalcCache(TCalcCache* & x) {
  void * tmp = x;
  #pragma offload target(mic:0) in(tmp)
  {
    auto * p = reinterpret_cast<TCalcPHI*>(tmp);
    delete p;
  }
  x = 0;
}
#endif
*/

/*
// TPWAFunctionGPU::TPWAFunctionGPU(string ConfigFileName) {
//   Init(ConfigFileName);
// }
#ifdef __INTEL_COMPILER
TPWAFunctionPHI::TPWAFunctionPHI(string ConfigFileName) {
  Init(ConfigFileName);
}
#endif
*/
void TPWAFunction::Init(string ConfigFileName) {
  std::cout<<"Init\n";
  ConfigParser conf(ConfigFileName, &resonances, &inputFiles);/*
  if(verbosity >= 3)
    resonances.PrintParameters();
  
  NCaches();
  data = new TDataCache(&resonances, inputFiles.dataEventFile, inputFiles.dataCacheFile);
  mem_alloc = data->mem_alloc;
  calc_data.resize(n_caches, 0);
  for(unsigned thr = 0; thr < n_caches; thr++) {
    unsigned blockSize = data->NEv()/n_caches;
    unsigned offset = thr*blockSize;
    if(thr == n_caches-1) blockSize = data->NEv()-offset;
    calc_data[thr] = CreateCalcCache(data, offset, blockSize);
  }
  PrintMem(data->NEv());
  mc = new TDataCache(&resonances, inputFiles.mcEventFile, inputFiles.mcCacheFile);
  mem_alloc = mc->mem_alloc;
  calc_mc.resize(n_caches, 0);
  for(unsigned thr = 0; thr < n_caches; thr++) {
    unsigned blockSize = mc->NEv()/n_caches;
    unsigned offset = thr*blockSize;
    if(thr == n_caches-1) blockSize = mc->NEv()-offset;
    calc_mc[thr] = CreateCalcCache(mc, offset, blockSize);
  }
  PrintMem(mc->NEv());
  */
}
/*
TPWAFunction::~TPWAFunction() {
  for(unsigned thr = 0; thr < n_caches; thr++) {
    DestroyCalcCache(calc_data[thr]);
    DestroyCalcCache(calc_mc[thr]);
  }
  delete data;
  delete mc;
}
#ifdef __INTEL_COMPILER
TPWAFunctionPHI::~TPWAFunctionPHI() {
  for(unsigned thr = 0; thr < n_caches; thr++) {
    DestroyCalcCache(calc_data[thr]);
    DestroyCalcCache(calc_mc[thr]);
  }
}
#endif
void TPWAFunction::Norm(double * par) {
  double start = omp_get_wtime();
  norm = 0;
  TFloat sum_w = 0;
  #pragma omp parallel
  {
    unsigned thr = omp_get_thread_num();
    TFloat t = 0;
    TFloat * t_N1 = 0;
    if(N1) t_N1 = new TFloat[NPar()];
    calc_mc[thr]->SetParameters(par);
    calc_mc[thr]->Norm(t, deltaPar, t_N1);
    #pragma omp atomic
      norm += t;
    if(N1) {
      for(unsigned i = 0; i < NPar(); i++) {
        #pragma omp atomic
          N1[i] += t_N1[i];
      }
      delete [] t_N1;
    }
    TFloat w = calc_mc[thr]->SumW();
    #pragma omp atomic
      sum_w += w;
  }
  if(N1)
    for(unsigned i = 0; i < NPar(); i++)
      N1[i] /= norm;
  norm /= sum_w;
  if(verbosity >= 2)
    cerr<<"Time of TCalcCPU::Norm calculation "<<omp_get_wtime()-start
        <<", dcs = "<<norm<<endl;
}

void TPWAFunction::Eval(double * par, double * grad, double * Z) {
  double start = omp_get_wtime();
  last_val = 0;
  if(grad) memset(grad, 0, NPar()*sizeof(double));
  if(Z)    memset(Z, 0, NZ()*sizeof(double));
  #pragma omp parallel
  {
    unsigned thr = omp_get_thread_num();
    TFloat t = 0;
    TFloat * t_grad = 0, * t_Z = 0;
    if(grad) t_grad = new TFloat[NPar()];
    if(Z) t_Z = new TFloat[NZ()];
    calc_data[thr]->SetParameters(par);
    calc_data[thr]->FCN(norm, t, deltaPar, N1, t_grad, t_Z);
    #pragma omp atomic
      last_val += t;
    if(grad) {
      for(unsigned i = 0; i < NPar(); i++) {
        #pragma omp atomic
          grad[i] += t_grad[i];
      }
      delete [] t_grad;
    }
    if(Z) {
      for(unsigned i = 0; i < NZ(); i++) {
        #pragma omp atomic
          Z[i] += t_Z[i];
      }
      delete [] t_Z;
    }
  }
  if(verbosity >= 2)
    cerr<<"Time of TCalcCPU::Eval calculation "<<omp_get_wtime()-start
        <<", dcs = "<<last_val<<endl;
}
#ifdef __INTEL_COMPILER
void TPWAFunctionPHI::Norm(double * par) {
  double start = omp_get_wtime();
  bool do_grad = N1;
  unsigned n_par = NPar();
  TFloat o_norm = 0;
  TFloat * o_deltaPar = new TFloat[n_par];
  TFloat * o_N1 = new TFloat[n_par];
  if(do_grad) {
    memcpy(o_deltaPar, deltaPar, n_par*sizeof(TFloat));
    memcpy(o_N1, N1, n_par*sizeof(TFloat));
  }
  TFloat sum_w = 0;
  TCalcCache ** o_calc_mc = calc_mc.data();
  unsigned sz_calc_mc = calc_mc.size()*sizeof(TCalcCache*);
  #pragma offload target(mic:0) in(o_calc_mc:length(sz_calc_mc)) \
      in(do_grad, n_par, o_norm, sum_w) in(par:length(n_par)) \
      in(o_deltaPar:length(n_par)) in(o_N1:length(n_par)) \
      out(o_norm) out(o_N1:length(n_par))
  {
  #pragma omp parallel
  {
    unsigned thr = omp_get_thread_num();
    TFloat t = 0;
    TFloat * t_N1 = 0;
    if(do_grad) t_N1 = new TFloat[n_par];
    o_calc_mc[thr]->SetParameters(par);
    o_calc_mc[thr]->Norm(t, o_deltaPar, t_N1);
    #pragma omp atomic
      o_norm += t;
    if(do_grad) {
      for(unsigned i = 0; i < n_par; i++) {
        #pragma omp atomic
          o_N1[i] += t_N1[i];
      }
      delete [] t_N1;
    }
    TFloat w = o_calc_mc[thr]->SumW();
    #pragma omp atomic
      sum_w += w;
  }
  if(do_grad)
    for(unsigned i = 0; i < n_par; i++)
      o_N1[i] /= o_norm;
  o_norm /= sum_w;
  }
  norm = o_norm;
  if(do_grad)
    memcpy(N1, o_N1, n_par*sizeof(TFloat));
  delete [] o_deltaPar;
  delete [] o_N1;
  if(verbosity >= 2)
    cerr<<"Time of TCalcCPU::Norm calculation "<<omp_get_wtime()-start
        <<", dcs = "<<norm<<endl;
}

void TPWAFunctionPHI::Eval(double * par, double * grad, double * Z) {
  double start = omp_get_wtime();
  bool do_grad = grad;
  bool do_Z = Z;
  unsigned n_par = NPar();
  unsigned n_Z = NZ();
  TFloat o_norm = norm;
  TFloat o_val = 0;
  TFloat * o_deltaPar = new TFloat[n_par];
  TFloat * o_N1 = new TFloat[n_par];
  double * o_grad = new TFloat[n_par];
  double * o_Z = new TFloat[n_Z];
  if(do_grad) {
    memcpy(o_deltaPar, deltaPar, n_par*sizeof(TFloat));
    memcpy(o_N1, N1, n_par*sizeof(TFloat));
    memset(o_grad, 0, n_par*sizeof(double));
  }
  if(do_Z)
    memset(o_Z, 0, n_Z*sizeof(double));
  TCalcCache ** o_calc_data = calc_data.data();
  unsigned sz_calc_data = calc_data.size()*sizeof(TCalcCache*);
  #pragma offload target(mic:0) in(o_calc_data:length(sz_calc_data)) \
      in(do_grad, do_Z, n_par, n_Z, o_norm, o_val) in(par:length(n_par)) \
      in(o_deltaPar:length(n_par)) in(o_N1:length(n_par)) \
      in(o_grad:length(n_par)) in(o_Z:length(n_Z)) \
      out(o_val) out(o_grad:length(n_par)) out(o_Z:length(n_Z))
  {
  #pragma omp parallel
  {
    unsigned thr = omp_get_thread_num();
    TFloat t = 0;
    TFloat * t_grad = 0, * t_Z = 0;
    if(do_grad) t_grad = new TFloat[n_par];
    if(do_Z) t_Z = new TFloat[n_Z];
    o_calc_data[thr]->SetParameters(par);
    o_calc_data[thr]->FCN(o_norm, t, o_deltaPar, o_N1, t_grad, t_Z);
    #pragma omp atomic
      o_val += t;
    if(do_grad) {
      for(unsigned i = 0; i < n_par; i++) {
        #pragma omp atomic
          o_grad[i] += t_grad[i];
      }
      delete [] t_grad;
    }
    if(do_Z) {
      for(unsigned i = 0; i < n_Z; i++) {
        #pragma omp atomic
          o_Z[i] += t_Z[i];
      }
      delete [] t_Z;
    }
  }
  }
  last_val = o_val;
  if(do_grad) memcpy(grad, o_grad, n_par*sizeof(double));
  if(do_Z)    memcpy(Z, o_Z, n_Z*sizeof(double));
  delete [] o_deltaPar;
  delete [] o_N1;
  delete [] o_grad;
  delete [] o_Z;
  if(verbosity >= 2)
    cerr<<"Time of TCalcCPU::Eval calculation "<<omp_get_wtime()-start
        <<", dcs = "<<last_val<<endl;
}
#endif
void TPWAFunction::Eval(double * par, double & val, double * grad, double * Z) {
  if(verbosity >= 3)
    fprintf(stderr, "Calculating minimization function\n");
  double start = omp_get_wtime();
  resonances.SetParameters(par);
  if(verbosity >= 2)
    for(unsigned r = 0; r < resonances.N(); r++) {
      for(unsigned i = 0; i < TResonance::nPar; i++)
        fprintf(stderr, "%12.6f", static_cast<TFloat>(resonances.Res(r).Par(i)));
      fprintf(stderr, "\n");
      fflush(stderr);
    }
  if(verbosity >= 3)
    resonances.PrintChanged();
  deltaPar = 0;
  N1 = 0;
  if(grad) {
    deltaPar = new TFloat[NPar()];
    for(unsigned i = 0; i < NPar(); i++)
      deltaPar[i] = IsFixed(i)?0.:grad_step; // TODO variable step
    N1 = new TFloat[NPar()];
    memset(N1, 0, NPar()*sizeof(TFloat));
  }
  Norm(par);
  Eval(par, grad, Z);
  if(grad) {
    delete [] deltaPar;
    delete [] N1;
  }
  if(verbosity >= 1)
    fprintf(stderr, "Time of minimization function (F%s%s) call %8.5f, fval = %12.4f\n",
            grad?"G":"", Z?"Z":"", omp_get_wtime()-start, last_val);
  val = last_val;
}

TFloat TPWAFunction::Fit(ROOTMinimizer::Strategy s) {
  TFloat start = omp_get_wtime();
  ROOTMinimizer minimizer(s, this);
  minimizer.tolerance = 0.5;
  minimizer.Minimize(50000);
  return omp_get_wtime()-start;
}

TFloat TPWAFunction::Test(unsigned n, double * _grad) {
  TFloat start = omp_get_wtime();
  double * par_orig = new double[NPar()];
  resonances.GetParameters(par_orig);
  double * par = new double[NPar()];
  memcpy(par, par_orig, NPar()*sizeof(double));
  double val = 0;
  Eval(par, val);
//   resonances.PrintParameters();
  for(unsigned k = 0; k < n; k++) {
    for(unsigned i = 0; i < NPar(); i++) {
      par[i] += grad_step;
      double val_g = 0;
      Eval(par, val_g);
//       fprintf(stderr, "  gradient[%2i] = %16.8f\n", i, (val_g-val)/grad_step);
      if(_grad) _grad[i] = (val_g-val)/grad_step;
//       fflush(stderr);
      par[i] = par_orig[i];
    }
  }
  Eval(par_orig, val);
//   resonances.PrintParameters();
  delete [] par;
  delete [] par_orig;
  return omp_get_wtime()-start;
}

TFloat TPWAFunction::TestGrad(unsigned n, unsigned nthr, double * _grad) {
  TFloat start = omp_get_wtime();
  double * par_orig = new double[NPar()];
  resonances.GetParameters(par_orig);
  double * par = new double[NPar()];
  memcpy(par, par_orig, NPar()*sizeof(double));
  double val = 0;
  double * grad = new double[NPar()];
  for(unsigned k = 0; k < n; k++) {
    TFloat start_c = omp_get_wtime();
    Eval(par, val, grad);
    for(unsigned i = 0; i < NPar(); i++) {
      if(verbosity >= 0) {
        fprintf(stderr, "%16.8f", grad[i]);
        if(i%5 == 4) fprintf(stderr, "\n");
      }
      if(_grad) _grad[i] = grad[i];
    }
    if(verbosity >= 0) fflush(stderr);
    fprintf(stderr, "%3i%9.5f\n", nthr, omp_get_wtime()-start_c);
    fflush(stderr);
  }
//   for(unsigned k = 0; k < n; k++) {
//     for(unsigned i = 0; i < NPar(); i++) {
//       par[i] += 0.01;
//     }
//     Eval(par, val, grad);
//     for(unsigned i = 0; i < NPar(); i++)
//       par[i] -= 0.01;
//     Eval(par, val, grad);
//   }
//   resonances.SetParameters(par_orig);
//   Eval(par_orig, val);
  delete [] par;
  delete [] par_orig;
  delete [] grad;
  return omp_get_wtime()-start;
}

TFloat TPWAFunction::TestGZ(unsigned n) {
  TFloat start = omp_get_wtime();
  double * par_orig = new double[NPar()];
  resonances.GetParameters(par_orig);
  double * par = new double[NPar()];
  memcpy(par, par_orig, NPar()*sizeof(double));
  double val = 0;
  double * grad = new double[NPar()];
  double * Z = new double[NZ()];
  for(unsigned k = 0; k < n; k++) {
    Eval(par, val, grad, Z);
    for(unsigned i = 0; i < NPar(); i++)
      fprintf(stderr, "  gradient[%2i] = %16.8f\n", i, grad[i]);
//     for(unsigned i = 0; i < NZ(); i++)
//       fprintf(stderr, "  Z[%3i] = %16.8f\n", i, Z[i]);
    fflush(stderr);
  }
//   for(unsigned k = 0; k < n; k++) {
//   for(unsigned i = 0; i < NPar(); i++) {
//   par[i] += 0.01;
// }
//   Eval(par, val, grad);
//   for(unsigned i = 0; i < NPar(); i++)
//   par[i] -= 0.01;
//   Eval(par, val, grad);
// }
//   resonances.SetParameters(par_orig);
  Eval(par_orig, val);
  delete [] par;
  delete [] par_orig;
  delete [] grad;
  delete [] Z;
  return omp_get_wtime()-start;
}
*/
