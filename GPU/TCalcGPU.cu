
#include "TCalcGPU.h"
#include "GPUMemoryHandler.h"
#include "Kernels.h"

TCalcGPU::TCalcGPU(TCache * x, unsigned offset, unsigned _nev)
: TCalcCache(x, offset, _nev) {
  memHandler = GPUMemoryHandler::Get();
  nThr    = GPUMemoryHandler::Get()->NThr();
  nBlocks = GPUMemoryHandler::Get()->CalcBlockDim(nev);
  Allocate();
  resInst->Changed();
  Recalc();
}

TCalcGPU::~TCalcGPU() {
  TCache::Cleanup();
}

template<class T>
void ToGPU(TPtr<T> & p, unsigned & mem_alloc) {
  if(!p) return;
  p.SetMemHandler(GPUMemoryHandler::Get());
  mem_alloc += p.AllocateCopy();
  GPUMemoryHandler::Get()->ToGPU(p.o, p.p, p.n*sizeof(T));
  p.o = 0;
}

template<class T>
void ToGPU(TPtrVec<T> & v, unsigned & mem_alloc) {
  for(unsigned i = 0; i < v.size(); i++)
    ToGPU(v[i], mem_alloc);
}

void TCalcGPU::Allocate() {
  ToGPU(v1kT2, mem_alloc);
  ToGPU(s_kchan, mem_alloc);
  ToGPU(weight, mem_alloc);
  ToGPU(precalc_factor, mem_alloc);
  Cleanup();
  bw.SetMemHandler(memHandler);
  blw_dec.SetMemHandler(memHandler);
  var_factor.SetMemHandler(memHandler);
  ampl.SetMemHandler(memHandler);
  ampl_sum.SetMemHandler(memHandler);
  for(unsigned r = 0; r < resInst->N(); r++) {
    for(int k = 0; k < 3; k++) {
      if(resInst->Res(r).SymmetryMultiplier(k) == 0.) continue;
      mem_alloc += bw[r*3+k].Allocate(nev);
      mem_alloc += blw_dec[r*3+k].Allocate(nev);
    }
  }
  prod_factor.Allocate(resInst->N()*3);
  mem_alloc += var_factor.Allocate(nev);
  for(unsigned r = 0; r < resInst->N(); r++) {
    for(unsigned m = 0; m < 4; m++) {
      if(!gReaction->Dens(m)) continue;
      mem_alloc += ampl[r*4+m].Allocate(nev);
    }
  }
  mem_alloc += ampl_sum.Allocate(nev);
}

void TCalcGPU::BWConstW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat M2, TFloat MG) {
  Kernels::BWConstW<<<nBlocks, nThr>>>(nev, reinterpret_cast<ccfloat*>(bw_rk), s_kchan_k, M2, MG);
//   Kernels::TestComplex<<<nBlocks, nThr>>>(reinterpret_cast<ccfloat*>(bw_rk), s_kchan_k, nev);
}

void TCalcGPU::BWVarW(cfloat * bw_rk, TFloat * s_kchan_k, TFloat * v1kT2_k,
                                 TFloat M2, TFloat MGRho, TFloat r, unsigned J) {
  Kernels::BWVarW<<<nBlocks, nThr>>>(nev, reinterpret_cast<ccfloat*>(bw_rk),
                                     s_kchan_k, v1kT2_k, M2, MGRho, r, J);
}

void TCalcGPU::BlWDec(TFloat * blw_dec_rk, TFloat * v1kT2_k, TFloat r, unsigned J) {
  Kernels::BlWDec<<<nBlocks, nThr>>>(nev, blw_dec_rk, v1kT2_k, r, J);
}

void TCalcGPU::VarFactor(cfloat * bw_rk, TFloat * blw_dec_rk, cfloat prod_factor) {
  Kernels::VarFactor<<<nBlocks, nThr>>>(nev, reinterpret_cast<ccfloat*>(var_factor.p),
                                        reinterpret_cast<ccfloat*>(bw_rk), blw_dec_rk,
                                        ccfloat(prod_factor.real(), prod_factor.imag()));
}

void TCalcGPU::Ampl(cfloat * ampl_rm, cfloat * precalc_factor_rkm) {
  Kernels::Ampl<<<nBlocks, nThr>>>(nev, reinterpret_cast<ccfloat*>(ampl_rm),
                                   reinterpret_cast<ccfloat*>(precalc_factor_rkm),
                                   reinterpret_cast<ccfloat*>(var_factor.p));
}

void TCalcGPU::AddAmpl(cfloat * ampl_rm) {
  Kernels::AddAmpl<<<nBlocks, nThr>>>(nev, reinterpret_cast<ccfloat*>(ampl_sum.p),
                                      reinterpret_cast<ccfloat*>(ampl_rm));
}

void TCalcGPU::_ConvergeAmpl(TFloat * t_ampl_sq, TFloat dens) {
  Kernels::ConvergeAmpl<<<nBlocks, nThr>>>(nev, dens, t_ampl_sq,
                                           reinterpret_cast<ccfloat*>(ampl_sum.p));
}

void TCalcGPU::ChangeSign(TFloat * t_ampl_sq) {
  Kernels::ChangeSign<<<nBlocks, nThr>>>(nev, t_ampl_sq);
}

TFloat TCalcGPU::_ConvergeNorm(TFloat dens) {
  TPtr<TFloat> N(memHandler);
  N.Allocate(nev);
  Kernels::ConvergeNorm<<<nBlocks, nThr>>>(nev, dens, N, reinterpret_cast<ccfloat*>(ampl_sum.p), weight);
  return GPUMemoryHandler::Get()->Reduce(N, nev);
}

TFloat TCalcGPU::LogLikelihood(TFloat * t_ampl_sq) {
  TPtr<TFloat> val(memHandler);
  val.Allocate(nev);
  Kernels::Log<<<nBlocks, nThr>>>(nev, val, t_ampl_sq, weight);
  return GPUMemoryHandler::Get()->Reduce(val, nev);
}

TFloat TCalcGPU::CalcF1(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq) {
  TPtr<TFloat> F1(memHandler);
  F1.Allocate(nev);
  Kernels::F1<<<nBlocks, nThr>>>(nev, F1, t_ampl_sq_i, t_ampl_sq, weight);
  return GPUMemoryHandler::Get()->Reduce(F1, nev);
}

TFloat TCalcGPU::CalcF2(TFloat * t_ampl_sq_i, TFloat * t_ampl_sq_j, TFloat * t_ampl_sq) {
  TPtr<TFloat> F2(memHandler);
  F2.Allocate(nev);
  Kernels::F2<<<nBlocks, nThr>>>(nev, F2, t_ampl_sq_i, t_ampl_sq_j, t_ampl_sq, weight);
  return GPUMemoryHandler::Get()->Reduce(F2, nev);
}
