
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

TPWAFunctionPHI::~TPWAFunctionPHI() {
  for(unsigned thr = 0; thr < n_caches; thr++) {
    DestroyCalcCache(calc_data[thr]);
    DestroyCalcCache(calc_mc[thr]);
  }
}

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
