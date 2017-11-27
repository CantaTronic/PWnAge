
#pragma once

#include "OffloadDirectives.h"
#include "TCache.h"
#include "TResonanceSet.h"

PWA_START_OFFLOAD
class TCalcCache: public TCache {
public:
  TCalcCache();
  TCalcCache(TCache * x, unsigned offset = 0, unsigned _nev = 0);
  virtual ~TCalcCache();
 /* virtual void FCN(const TFloat norm, TFloat & val, const TFloat * D = 0, const TFloat * N1 = 0, TFloat * grad = 0, TFloat * Z = 0);
  virtual void Norm(TFloat & val, const TFloat * D = 0, TFloat * N1 = 0);
  virtual TFloat SumW() { return sum_w; }
  virtual void SetParameters(double * par);
protected:
  virtual void Allocate() = 0;
  virtual void BWConstW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat M2, TFloat MG) = 0;
  virtual void BWVarW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat * v1kT2_k,
                      TFloat M2, TFloat MGRho, TFloat r, unsigned J) = 0;
  virtual void BlWDec(TFloat * blw_dec_rk, TFloat * v1kT2_k, TFloat r, unsigned J) = 0;
  virtual void VarFactor(cfloat * bw_rk, TFloat * blw_dec_rk, cfloat prod_factor) = 0;
  virtual void Ampl(cfloat * ampl_rm, cfloat * precalc_factor_rkm) = 0;
  virtual void Recalc();
  virtual void RecalcGrad(unsigned ipar, TFloat delta);
  virtual void AddAmpl(cfloat * ampl_rm) = 0;
  virtual void ConvergeAmpl(TFloat * t_ampl_sq);
  virtual void _ConvergeAmpl(TFloat * t_ampl_sq, TFloat dens) = 0;
  virtual void ChangeSign(TFloat * t_ampl_sq) = 0;
  virtual TFloat ConvergeNorm();
  virtual TFloat _ConvergeNorm(TFloat dens) = 0;
  virtual TFloat LogLikelihood(TFloat * t_ampl_sq) = 0;
  virtual TFloat CalcF1(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq) = 0;
  virtual TFloat CalcF2(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq_j, TFloat * t_ampl_sq) = 0;
  virtual void Cleanup();
  TPtrVec<cfloat> bw;
  TPtrVec<TFloat> blw_dec;
  TPtr<cfloat> prod_factor;
  TPtr<cfloat> var_factor;
  TPtrVec<cfloat> ampl;
  TPtr<cfloat> ampl_sum;
  TResonanceSet * resInst;
  TFloat prev_val;
  TFloat sum_w;
private:
  TCalcCache(const TCalcCache & x);              // copy-constructor
  TCalcCache & operator= (const TCalcCache & x); // copy-assignment
//   TCalcCache (TCalcCache && x);                  // move-constructor
//   TCalcCache & operator= (TCalcCache&& x);       // move-assignment*/
};
PWA_END_OFFLOAD
