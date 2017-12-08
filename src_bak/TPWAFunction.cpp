
#include "TPWAFunction.h"

#include "TDataCache.h"
// 

#include "../libConfPars/ConfigParser.h"
#include "TPWAFunctionCPU.h"
#include <cstring>
#include <iostream>
using std::cerr;
using std::endl;
using std::cout;
#include <omp.h>
#include <stdexcept>
#include <typeinfo>

using std::domain_error;

int TPWAFunction::verbosity = 1;
double TPWAFunction::grad_step = 1e-5;

void TPWAFunction::DestroyCalcCache(TCalcCache* & x) {
  delete x;
}

TPWAFunction  * TPWAFunction::Create (deviceType dev, string ConfigFileName){
  /* Factory method. Create a concrete enable child object
   * TODO: Add creation for GPU and PHI children.
   */
  switch (dev){
    case 1:
      cout<<"GPU"<<endl;
//       return new TPWAFunctionGPU;
      break;
    case 2:
      cout<<"PHI"<<endl;
//       return new TPWAFunctionPHI;
      break;
    default:
      cout<<"CPU"<<endl;
      return new TPWAFunctionCPU(ConfigFileName);
  };
}


TPWAFunction::TPWAFunction() {}

void TPWAFunction::Init(string ConfigFileName) {
  std::cout<<"Init "<<std::endl;
  ConfigParser conf(ConfigFileName, &resonances, &inputFiles);
//   if(verbosity >= 3)
//     resonances.PrintParameters();
  
//   NCaches();
//   data = new TDataCache(&resonances, inputFiles.dataEventFile, inputFiles.dataCacheFile);
//   mem_alloc = data->mem_alloc;
//   calc_data.resize(n_caches, 0);
//   for(unsigned thr = 0; thr < n_caches; thr++) {
//     unsigned blockSize = data->NEv()/n_caches;
//     unsigned offset = thr*blockSize;
//     if(thr == n_caches-1) blockSize = data->NEv()-offset;
//     calc_data[thr] = CreateCalcCache(data, offset, blockSize);
//   }
//   PrintMem(data->NEv());
//   mc = new TDataCache(&resonances, inputFiles.mcEventFile, inputFiles.mcCacheFile);
//   mem_alloc = mc->mem_alloc;
//   calc_mc.resize(n_caches, 0);
//   for(unsigned thr = 0; thr < n_caches; thr++) {
//     unsigned blockSize = mc->NEv()/n_caches;
//     unsigned offset = thr*blockSize;
//     if(thr == n_caches-1) blockSize = mc->NEv()-offset;
//     calc_mc[thr] = CreateCalcCache(mc, offset, blockSize);
//   }
//   PrintMem(mc->NEv());
  
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
*//*
TFloat TPWAFunction::Fit(ROOTMinimizer::Strategy s) {
  TFloat start = omp_get_wtime();
  ROOTMinimizer minimizer(s, this);
  minimizer.tolerance = 0.5;
  minimizer.Minimize(50000);
  return omp_get_wtime()-start;
}*/
/*
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
