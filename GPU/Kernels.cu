
#include "Kernels.h"

__device__ __forceinline__ TFloat pownd(TFloat x, int n) {
  if(n == 0) return 1;
  TFloat r = x;
  for(int i = 2; i <= n; i++) r *= x;
  return r;
}

__device__ __forceinline__ TFloat BlattWeisskopfFact(int l, TFloat kTsq, TFloat r) {
  TFloat dx = 1.;
  TFloat r2 = pownd(r/0.1973, 2);
  TFloat z  = kTsq*r2;
  if(l == 1)
    dx = 1. + z;
  if(l == 2)
    dx = 9. + 3.*z + z*z;
  if(l == 3)
    dx = 225. + 45.*z + 6.*z*z + pownd(z, 3);
  if(l == 4)
    dx = 11025. + 1575.*z + 135.*z*z + 10.*pownd(z, 3) + pownd(z, 4);
  if(l == 5)
    dx = 893025. + 99225.*z + 6300.*z*z + 315.*pownd(z, 3) + 15.*pownd(z, 4) + pownd(z, 5);
  if(l == 6)
    dx = 108056025. + 9823275.*z + 496125.*z*z + 18900.*pownd(z, 3) + 630.*pownd(z, 4)
        + 21.*pownd(z,5) + pownd(z,6);
  if(l >= 1) dx = dx/pownd(r2, l);
//      if(l >= 7) dx=dx*((1.+4.*q2)*r2)**(L-6)
  return dx;
}

__device__ __forceinline__ TFloat Rho(TFloat s, int J, TFloat qsq, TFloat r) {
  return 2 * sqrt(qsq/s) * pownd(qsq, J) / BlattWeisskopfFact(J, qsq, r);
}

// Simple Breit Wigner function with constant width
__global__ void Kernels::BWConstW(unsigned nev, ccfloat * bw_rk,
                                  TFloat * s_kchan_k, TFloat M2, TFloat MG) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  bw_rk[i] = 1. / ccfloat(s_kchan_k[i]-M2, MG);
}

// depends on res.M, res.G, res.r
__global__ void Kernels::BWVarW(unsigned nev, ccfloat * bw_rk, TFloat * s_kchan_k,
                                TFloat * v1kT2_k, TFloat M2, TFloat MGRho, TFloat r, unsigned J) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  bw_rk[i] = 1. / ccfloat(s_kchan_k[i]-M2, MGRho*Rho(s_kchan_k[i], J, -v1kT2_k[i], r));
}

__global__ void Kernels::BlWDec(unsigned nev, TFloat * blw_dec_rk,
                                TFloat * v1kT2_k, TFloat r, unsigned J) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  blw_dec_rk[i] = sqrt( BlattWeisskopfFact(J, -v1kT2_k[i], r) );
}

__global__ void Kernels::VarFactor(unsigned nev, ccfloat * var_factor,
                                   ccfloat * bw_rk, TFloat * blw_dec_rk, ccfloat prod_factor) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  var_factor[i] = prod_factor * bw_rk[i] / blw_dec_rk[i];
}

__global__ void Kernels::Ampl(unsigned nev, ccfloat * ampl_rm,
                              ccfloat * precalc_factor_rkm, ccfloat * var_factor) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  ampl_rm[i] += precalc_factor_rkm[i] * var_factor[i];
}

__global__ void Kernels::AddAmpl(unsigned nev, ccfloat * ampl_sum, ccfloat * ampl_rm) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  ampl_sum[i] += ampl_rm[i];
}

__global__ void Kernels::ConvergeAmpl(unsigned nev, TFloat jpsiDens,
                                      TFloat * t_ampl_sq, ccfloat * ampl_sum) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  TFloat re = real(ampl_sum[i]),
         im = imag(ampl_sum[i]);
  t_ampl_sq[i] += jpsiDens * (re*re + im*im);
}

__global__ void Kernels::ConvergeNorm(unsigned nev, TFloat jpsiDens, TFloat * N,
                                      ccfloat * ampl_sum, TFloat * weight) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  TFloat re = real(ampl_sum[i]),
         im = imag(ampl_sum[i]);
  N[i] = jpsiDens * (re*re + im*im) * weight[i];
}

__global__ void Kernels::Log(unsigned nev, TFloat * val, TFloat * ampl_sq, TFloat * weight) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  val[i] = - weight[i] * log(ampl_sq[i]);
}

__global__ void Kernels::ChangeSign(unsigned nev, TFloat * ampl_sq_ipar) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  ampl_sq_ipar[i] *= -1;
}

__global__ void Kernels::F1(unsigned nev, TFloat * F1, TFloat * ampl_sq_ipar,
                            TFloat * ampl_sq, TFloat * weight) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  F1[i] = - weight[i] * ampl_sq_ipar[i] / ampl_sq[i];
}

__global__ void Kernels::F2(unsigned nev, TFloat * F2, TFloat * ampl_sq_ipar,
                            TFloat * ampl_sq_jpar, TFloat * ampl_sq, TFloat * weight) {
  unsigned i = threadIdx.x + blockDim.x*blockIdx.x;
  if(i >= nev) return;
  F2[i] = weight[i] * ampl_sq_ipar[i] * ampl_sq_jpar[i] / (ampl_sq[i]*ampl_sq[i]);
}

__global__ void Kernels::Reduce(unsigned n, TFloat * p_in, TFloat * p_out) {
  extern __shared__ TFloat sdata[];
  // each thread loads one element from global to shared mem
  unsigned int ithr = threadIdx.x;
  unsigned int nthr = blockDim.x;
  unsigned int offset = nthr*2*blockIdx.x;
  unsigned int i = offset + ithr;
  unsigned int j = i + nthr;
  if(j < n)
    sdata[ithr] = p_in[i] + p_in[j];
  else if(i < n)
    sdata[ithr] = p_in[i];
  else
    sdata[ithr] = 0;
  __syncthreads();
  // do reduction in shared mem
  for(unsigned int s = nthr/2; s > 1; s >>= 1) {
    if(ithr < s) sdata[ithr] += sdata[ithr+s];
    __syncthreads();
  }
  // write result for this block to global mem
  if (ithr == 0) p_out[blockIdx.x] = sdata[0]+sdata[1];
}
