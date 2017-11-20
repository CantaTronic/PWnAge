
#include "GPUMemoryHandler.h"
#include "Kernels.h"
#include <cstdio>

void Err(cudaError_t err, const char * f);

GPUMemoryHandler::GPUMemoryHandler() {
  cudaDeviceProp deviceProp;
  cudaGetDeviceProperties(&deviceProp, 0);
  nThr = deviceProp.maxThreadsPerBlock;
}

GPUMemoryHandler * GPUMemoryHandler::Get() {
  static GPUMemoryHandler gInstance;
  return &gInstance;
}

void * GPUMemoryHandler::Allocate(unsigned n_bytes) {
  void * p;
  cudaError_t err = cudaMalloc(&p, n_bytes);
  Err(err, "Alloc");
  return p;
}

void GPUMemoryHandler::Free(void * p) {
  cudaError_t err = cudaFree(p);
  Err(err, "Free");
}

void GPUMemoryHandler::Zero(void * p, unsigned n_bytes) {
  cudaError_t err = cudaMemset(p, 0, n_bytes);
  Err(err, "Zero");
}

void GPUMemoryHandler::Copy(void * to, const void * from, unsigned n_bytes) {
  cudaError_t err = cudaMemcpy(to, from, n_bytes, cudaMemcpyDeviceToDevice);
  Err(err, "Copy");
}

void GPUMemoryHandler::ToGPU(void * p, void * p_d, unsigned nbytes) {
  cudaError_t err = cudaMemcpy(p_d, p, nbytes, cudaMemcpyHostToDevice);
  Err(err, "ToGPU");
}

void GPUMemoryHandler::FromGPU(void * p, void * p_d, unsigned nbytes) {
  cudaError_t err = cudaMemcpy(p, p_d, nbytes, cudaMemcpyDeviceToHost);
  Err(err, "FromGPU");
}

TFloat GPUMemoryHandler::Reduce(TFloat * p_in, unsigned n) {
  while(n > 1) {
    unsigned nBlocks = CalcBlockDim(n);
    TFloat * p_out = _Allocate<TFloat>(nBlocks);
    Kernels::Reduce<<<nBlocks, nThr, nThr*sizeof(TFloat)>>>(n, p_in, p_out);
    Free(p_in);
    p_in = p_out;
    n = nBlocks;
  }
  TFloat ret = 0;
  FromGPU(&ret, p_in, sizeof(TFloat));
  Free(p_in);
  return ret;
}

inline void Err(cudaError_t err, const char * f) {
  if(err != cudaSuccess) {
    fprintf(stderr, "GPUMemoryHandler::%s: error \"%s\"\n", f, cudaGetErrorString(err));
    fflush(stderr);
    exit(-1);
  }
}
