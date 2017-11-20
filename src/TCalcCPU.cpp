
#include "TCalcCPU.h"
#include "BlattWeisskopf.h"

TCalcCPU::TCalcCPU(): TCalcCache() { }

TCalcCPU::TCalcCPU(TCache * x, unsigned offset, unsigned _nev)
: TCalcCache(x, offset, _nev) {
  Allocate();
  resInst->Changed();
  Recalc();
}

void TCalcCPU::Allocate() {
  Cleanup();
  for(unsigned r = 0; r < resInst->N(); r++) {
    for(int k = 0; k < 3; k++) {
      if(resInst->Res(r).SymmetryMultiplier(k) == 0.) continue;
      mem_alloc += bw[r*3+k].Allocate(nev);
      mem_alloc += blw_dec[r*3+k].Allocate(nev);
    }
  }
  mem_alloc += prod_factor.Allocate(resInst->N()*3);
  mem_alloc += var_factor.Allocate(nev);
  for(unsigned r = 0; r < resInst->N(); r++) {
    for(unsigned m = 0; m < 4; m++) {
      if(!gReaction->Dens(m)) continue;
      mem_alloc += ampl[r*4+m].Allocate(nev);
    }
  }
  mem_alloc += ampl_sum.Allocate(nev);
}

void TCalcCPU::BWConstW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat M2, TFloat MG) {
  for(size_t i = 0; i < nev; i++)
    bw_rk[i] = 1. / cfloat(s_kchan_k[i]-M2, MG);
}

void TCalcCPU::BWVarW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat * v1kT2_k,
                                 TFloat M2, TFloat MGRho, TFloat r, unsigned J) {
  for(size_t i = 0; i < nev; i++)
    bw_rk[i] = 1. / cfloat(s_kchan_k[i]-M2, MGRho*Rho(s_kchan_k[i], J, -v1kT2_k[i], r));
}

void TCalcCPU::BlWDec(TFloat * blw_dec_rk, TFloat * v1kT2_k, TFloat r, unsigned J) {
  for(size_t i = 0; i < nev; i++)
    blw_dec_rk[i] = sqrt( BlattWeisskopfFact(J, -v1kT2_k[i], r) );
}

void TCalcCPU::VarFactor(cfloat * bw_rk, TFloat * blw_dec_rk, cfloat prod_factor) {
  for(size_t i = 0; i < nev; i++)
    var_factor[i] = prod_factor * bw_rk[i] / blw_dec_rk[i];
}

void TCalcCPU::Ampl(cfloat * ampl_rm, cfloat * precalc_factor_rkm) {
  for(size_t i = 0; i < nev; i++)
    ampl_rm[i] += precalc_factor_rkm[i] * var_factor[i];
}

void TCalcCPU::AddAmpl(cfloat * ampl_rm) {
  for(size_t i = 0; i < nev; i++)
    ampl_sum[i] += ampl_rm[i];
}

void TCalcCPU::_ConvergeAmpl(TFloat * t_ampl_sq, TFloat dens) {
  for(size_t i = 0; i < nev; i++) {
    TFloat re = real(ampl_sum[i]),
           im = imag(ampl_sum[i]);
    t_ampl_sq[i] += dens * (re*re + im*im);
  }
}

void TCalcCPU::ChangeSign(TFloat * t_ampl_sq) {
  for(size_t i = 0; i < nev; i++)
    t_ampl_sq[i] *= -1;
}

TFloat TCalcCPU::_ConvergeNorm(TFloat dens) {
  TFloat N = 0;
  for(size_t i = 0; i < nev; i++) {
    TFloat re = real(ampl_sum[i]),
           im = imag(ampl_sum[i]);
    N += dens * (re*re + im*im) * weight[i];
  }
  return N;
}

TFloat TCalcCPU::LogLikelihood(TFloat * t_ampl_sq) {
  TFloat val = 0;
  for(size_t i = 0; i < nev; i++)
    val -= weight[i] * log(t_ampl_sq[i]);
  return val;
}

TFloat TCalcCPU::CalcF1(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq) {
  TFloat F1 = 0;
  for(size_t i = 0; i < nev; i++)
    F1 -= weight[i] * t_ampl_sq_i[i] / t_ampl_sq[i];
  return F1;
}

TFloat TCalcCPU::CalcF2(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq_j, TFloat * t_ampl_sq) {
  TFloat F2 = 0;
  for(size_t i = 0; i < nev; i++)
    F2 += weight[i] * t_ampl_sq_i[i] * t_ampl_sq_j[i] / (t_ampl_sq[i]*t_ampl_sq[i]);
  return F2;
}
