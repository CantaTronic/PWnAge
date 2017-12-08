
#pragma once

#include "TFloat.h"
#include "cuda_complex.hpp"
typedef complex<TFloat> ccfloat;

namespace Kernels {
  // Simple Breit Wigner function with constant width
  __global__ void BWConstW(unsigned nev, ccfloat * bw_rk,
                           TFloat * s_kchan_k, TFloat M2, TFloat MG);
  // depends on res.M, res.G, res.r
  __global__ void BWVarW(unsigned nev, ccfloat * bw_rk, TFloat * s_kchan_k,
                          TFloat * v1kT2_k, TFloat M2, TFloat MGRho, TFloat r, unsigned J);
  __global__ void BlWDec(unsigned nev, TFloat * blw_dec_rk,
                         TFloat * v1kT2_k, TFloat r, unsigned J);
  __global__ void VarFactor(unsigned nev, ccfloat * var_factor,
                            ccfloat * bw_rk, TFloat * blw_dec_rk, ccfloat prod_factor);
  __global__ void Ampl(unsigned nev, ccfloat * ampl_rm,
                       ccfloat * precalc_factor_rkm, ccfloat * var_factor);
  __global__ void AddAmpl(unsigned nev, ccfloat * ampl_sum, ccfloat * ampl_rm);
  __global__ void ConvergeAmpl(unsigned nev, TFloat jpsiDens,
                               TFloat * t_ampl_sq, ccfloat * ampl_sum);
  __global__ void ConvergeNorm(unsigned nev, TFloat jpsiDens, TFloat * N,
                               ccfloat * ampl_sum, TFloat * weight);
  __global__ void Log(unsigned nev, TFloat * val, TFloat * ampl_sq, TFloat * weight);
  __global__ void ChangeSign(unsigned nev, TFloat * ampl_sq_ipar);
  __global__ void F1(unsigned nev, TFloat * F1, TFloat * ampl_sq_ipar,
                     TFloat * ampl_sq, TFloat * weight);
  __global__ void F2(unsigned nev, TFloat * F2, TFloat * ampl_sq_ipar,
                     TFloat * ampl_sq_jpar, TFloat * ampl_sq, TFloat * weight);
  __global__ void Reduce(unsigned n, TFloat * p_in, TFloat * p_out);
}
