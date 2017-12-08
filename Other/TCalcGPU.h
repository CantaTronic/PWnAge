
#pragma once

#include "TCalcCache.h"

class TCalcGPU: public TCalcCache {
public:
  TCalcGPU(TCache * x, unsigned offset = 0, unsigned _nev = 0);
  virtual ~TCalcGPU();
protected:
  virtual void Allocate();
  virtual void BWConstW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat M2, TFloat MG);
  virtual void BWVarW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat * v1kT2_k,
                      TFloat M2, TFloat MGRho, TFloat r, unsigned J);
  virtual void BlWDec(TFloat * blw_dec_rk, TFloat * v1kT2_k, TFloat r, unsigned J);
  virtual void VarFactor(cfloat * bw_rk, TFloat * blw_dec_rk, cfloat prod_factor);
  virtual void Ampl(cfloat * ampl_rm, cfloat * precalc_factor_rkm);
  virtual void AddAmpl(cfloat * ampl_rm);
  virtual void _ConvergeAmpl(TFloat * t_ampl_sq, TFloat dens);
  virtual void ChangeSign(TFloat * t_ampl_sq);
  virtual TFloat _ConvergeNorm(TFloat dens);
  virtual TFloat LogLikelihood(TFloat * t_ampl_sq);
  virtual TFloat CalcF1(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq);
  virtual TFloat CalcF2(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq_j, TFloat * t_ampl_sq);
  unsigned nThr, nBlocks;
private:
  TCalcGPU(const TCalcGPU & x);              // copy-constructor
  TCalcGPU & operator= (const TCalcGPU & x); // copy-assignment
};
