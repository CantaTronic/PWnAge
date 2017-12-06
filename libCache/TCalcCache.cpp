
#include "TCalcCache.h"
#include "BlattWeisskopf.h"
#include <cstring>

#ifdef __INTEL_COMPILER
  #pragma offload_attribute(push, target(mic))
#endif
#include <iostream>
#ifdef __INTEL_COMPILER
  #pragma offload_attribute(pop)
#endif

using std::cerr;
using std::endl;
#include <iomanip>
using std::setw;
#include <omp.h>
#include "TReaction.h"
TReaction * gReaction = 0;  //by 06.12.17

void TCalcCache::SetParameters(double * par) {
  resInst->SetParameters(par);
}

TCalcCache::TCalcCache()
: TCache(), prev_val(0), sum_w(0) { }

TCalcCache::TCalcCache(TCache * x, unsigned offset, unsigned _nev)
: TCache(*x), bw(resonances->N()*3), blw_dec(resonances->N()*3),
  ampl(resonances->N()*4), prev_val(0), sum_w(0) {
  // has it's own instance of TResonanceSet
  resInst = new TResonanceSet(*resonances);
  nev = _nev;
  if(_nev == 0) nev = x->NEv() - offset;
  if(_nev > x->NEv() - offset) {
    cerr<<" Warning: TJpsiToKKpi0Cache("<<x<<", "<<offset<<", "<<_nev<<"):"<<endl
        <<"  _nev == "<<_nev<<" > "<<x<<"->nev - offset == "<<x->NEv()<<" - "<<offset<<endl;
    nev = x->NEv() - offset;
  }
  TCache::Shift(offset);
  for(size_t i = 0; i < nev; i++)
    sum_w += weight[i];
}

TCalcCache::~TCalcCache() {
  delete resInst;
  Cleanup();
}

void TCalcCache::Cleanup() {
  bw.Cleanup();
  blw_dec.Cleanup();
  prod_factor.Cleanup();
  var_factor.Cleanup();
  ampl.Cleanup();
  ampl_sum.Cleanup();
}

void TCalcCache::Recalc() {
  double start = omp_get_wtime();
  for(unsigned int r = 0; r < resInst->N(); r++) {
    // find changed parameter
    TResonance res = resInst->Res(r);
    bool recalc_bw = res._M().Changed() || res._G().Changed() ||
                    (res.type() == 1 && res._r().Changed()),
         recalc_blw_dec = res._r().Changed(),
         recalc_prod_factor = res._Coupling().Changed() || res._Phase().Changed();
    // recalculate bw[i,k,r], blw_dec[i,k,r], prod_factor[r]
    if(recalc_bw) { // recalculate bw[i,k,r]
      double start = omp_get_wtime();
      TFloat M2 = res.M()*res.M();
      TFloat MG = res.M()*res.G();
      for(int k = 0; k < 3; k++) {
        if(bw[r*3+k] == 0) continue;
        if(res.type() == 0) {
          // Simple Breit Wigner function with constant width
          BWConstW(bw[r*3+k], s_kchan[k], M2, MG);
        } else {
          // depends on res.M, res.G, res.r
          TFloat q2_M = q2(M2, gReaction->Mass(k, 0), gReaction->Mass(k, 1));
          TFloat MGRho = MG/Rho(M2, res.J(), q2_M, res.r());
          BWVarW(bw[r*3+k], s_kchan[k], v1kT2[k], M2, MGRho, res.r(), res.J());
        }
      }
      if(verbosity >= 3)
        cerr<<"Time of TCalcCache::RecalcBW calculation "<<omp_get_wtime()-start<<endl;
    }
    if(recalc_blw_dec) { // recalculate blw_dec[i,k,r]
      double start = omp_get_wtime();
      for(int k = 0; k < 3; k++) {
        if(blw_dec[r*3+k] == 0) continue;
        BlWDec(blw_dec[r*3+k], v1kT2[k], res.r(), res.J());
      }
      if(verbosity >= 3)
        cerr<<"Time of TCalcCache::RecalcBlWDec calculation "<<omp_get_wtime()-start<<endl;
    }
    // actualy prod_factor is the same for both caches, but the calculation is lightweight,
    // so for code readability we keep a separate copy of it in each cache
    if(recalc_prod_factor) {
      cfloat tmp = res.Coupling() * exp(cfloat(0, res.Phase()));
      for(int k = 0; k < 3; k++)
        prod_factor[r*3+k] = tmp * res.SymmetryMultiplier(k);
    }
    // recalculate ampl[r][k][i][m]
    if(recalc_bw || recalc_blw_dec || recalc_prod_factor) {
      double start = omp_get_wtime();
      for(int m = 0; m < 4; m++) {
        if(!gReaction->Dens(m)) continue;
        ampl[r*4+m].Zero();
      }
      for(int k = 0; k < 3; k++) {
        if(bw[r*3+k] == 0) continue;
        VarFactor(bw[r*3+k], blw_dec[r*3+k], prod_factor[r*3+k]);
        for(int m = 0; m < 4; m++) {
          if(!gReaction->Dens(m)) continue;
          Ampl(ampl[r*4+m], precalc_factor[(res.J()-1)*3*4+k*4+m]);
        }
      }
      if(verbosity >= 3)
        cerr<<"Time of TCalcCache::RecalcAmpl calculation "<<omp_get_wtime()-start<<endl;
    }
  }
  if(verbosity >= 3)
    cerr<<"Time of Recalc() calculation "<<omp_get_wtime()-start
        <<" with thread "<<omp_get_thread_num()<<" of "<<omp_get_max_threads()<<endl;
}

void TCalcCache::RecalcGrad(unsigned ipar, TFloat delta) {
  double start = omp_get_wtime();
  unsigned r = ipar/TResonance::nPar;
  TResonance res = resInst->Res(r);
  TFitPar par = resInst->Par(ipar);
  TFloat par_save = par;
  par = par_save+delta;
  bool recalc_bw = res._M().Changed() || res._G().Changed() || (res.type() == 1 && res._r().Changed()),
       recalc_blw_dec = res._r().Changed(),
       recalc_prod_factor = res._Coupling().Changed() || res._Phase().Changed();
  if(recalc_bw) {
    for(int k = 0; k < 3; k++) {
      if(res.SymmetryMultiplier(k) == 0) continue;
      bw[r*3+k].AllocateCopy();
    }
  }
  if(recalc_blw_dec) {
    for(int k = 0; k < 3; k++) {
      if(res.SymmetryMultiplier(k) == 0) continue;
      blw_dec[r*3+k].AllocateCopy();
    }
  }
  if(recalc_bw || recalc_blw_dec || recalc_prod_factor) {
    for(int m = 0; m < 4; m++) {
      if(!gReaction->Dens(m)) continue;
      ampl[r*4+m].AllocateCopy();
    }
  }
  Recalc();
  par = par_save;
  par.SetUnchanged(); // to set `changed' to false
  if(recalc_bw)
    bw.DropCopy();
  if(recalc_blw_dec)
    blw_dec.DropCopy();
  if(recalc_prod_factor) {
    cfloat tmp = res.Coupling() * exp(cfloat(0, res.Phase()));
    for(int k = 0; k < 3; k++)
      prod_factor[r*3+k] = tmp * res.SymmetryMultiplier(k);
  }
  if(verbosity >= 3)
    cerr<<"Time of RecalcGrad() calculation "<<omp_get_wtime()-start
        <<" with thread "<<omp_get_thread_num()<<" of "<<omp_get_max_threads()<<endl;
}

void TCalcCache::ConvergeAmpl(TFloat * t_ampl_sq) {
  for(int m = 0; m < 4; m++) {
    if(!gReaction->Dens(m)) continue;
    ampl_sum.Zero();
    for(unsigned r = 0; r < resInst->N(); r++)
      AddAmpl(ampl[r*4+m]);
    _ConvergeAmpl(t_ampl_sq, gReaction->Dens(m));
  }
}

TFloat TCalcCache::ConvergeNorm() {
  TFloat N = 0;
  for(int m = 0; m < 4; m++) {
    if(!gReaction->Dens(m)) continue;
    ampl_sum.Zero();
    for(unsigned r = 0; r < resInst->N(); r++)
      AddAmpl(ampl[r*4+m]);
    N += _ConvergeNorm(gReaction->Dens(m));
  }
  return N;
}

void TCalcCache::Norm(TFloat & val, const TFloat * D, TFloat * N1) {
  Recalc();
  double start = omp_get_wtime();
  prev_val = val = ConvergeNorm();
  // for gradients
  if(N1) {
    for(unsigned ipar = 0; ipar < resInst->NPar(); ipar++) {
      if(D[ipar] == 0.) {
        N1[ipar] = 0.;
        continue;
      }
      RecalcGrad(ipar, D[ipar]);
      N1[ipar] = ConvergeNorm()-val;
      ampl.DropCopy();
    }
  }
  if(verbosity >= 3)
    cerr<<"Time of TCalcCache::FCN calculation "<<omp_get_wtime()-start
        <<" with thread "<<omp_get_thread_num()<<" of "<<omp_get_max_threads()<<endl;
}

void TCalcCache::FCN(const TFloat norm, TFloat & val, const TFloat * D, const TFloat * N1, TFloat * grad, TFloat * Z) {
  Recalc();
  double start = omp_get_wtime();
  TPtr<TFloat> ampl_sq(memHandler);
  ampl_sq.Allocate(nev);
  ampl_sq.Zero();
  ConvergeAmpl(ampl_sq);
  // obj. func.
  val = LogLikelihood(ampl_sq) + log(norm)*SumW();
  prev_val = val;
  // gradients & 2nd deriv~s
  if(grad) {
    unsigned n_par = resInst->NPar();
    memset(grad, 0, n_par*sizeof(TFloat));
    if(Z) memset(Z, 0, resInst->NZ()*sizeof(TFloat));
    TPtrVec<TFloat> ampl_sq_p(n_par, memHandler);
    vector<TFloat> F1(n_par, 0);
    for(unsigned ipar = 0, iz = 0; ipar < n_par; ipar++) {
      if(D[ipar] == 0.) continue;
      RecalcGrad(ipar, D[ipar]);
      ampl_sq_p[ipar].Allocate(nev);
      TFloat * ampl_sq_ipar = ampl_sq_p[ipar];
      ampl_sq_p[ipar].FillWith(ampl_sq);
      ChangeSign(ampl_sq_ipar);
      ConvergeAmpl(ampl_sq_ipar);
      ampl.DropCopy();
      F1[ipar] = CalcF1(ampl_sq_ipar, ampl_sq);
      grad[ipar] = (F1[ipar] + SumW()*N1[ipar]) / D[ipar];
//       fprintf(stderr,
//               "ipar = %2i, F1 = %16.8f, SumW = %16.8f, N1 = %16.8f, D = %16.8f, grad = %16.8f\n",
//               ipar, F1[ipar], SumW(), N1[ipar], D[ipar], grad[ipar]);
//       fflush(stderr);
      if(Z) {
        for(unsigned jpar = 0; jpar <= ipar; jpar++) {
          if(D[jpar] == 0.) continue;
          TFloat * ampl_sq_jpar = ampl_sq_p[jpar];
          TFloat F2 = CalcF2(ampl_sq_ipar, ampl_sq_jpar, ampl_sq);
          Z[iz] = (F2 + F1[ipar]*N1[jpar] + F1[jpar]*N1[ipar] + SumW()*N1[ipar]*N1[jpar])
                / (D[ipar]*D[ipar]);
          iz++;
        }
      }
    }
    ampl_sq_p.Cleanup();
  } // if(grad)
  ampl_sq.Cleanup();
  if(verbosity >= 3)
    cerr<<"Time of TCalcCache::FCN calculation "<<omp_get_wtime()-start
        <<" with thread "<<omp_get_thread_num()<<" of "<<omp_get_max_threads()<<endl;
}
