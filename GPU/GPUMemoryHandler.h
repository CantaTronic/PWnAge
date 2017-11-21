
#pragma once

#include "cfloat.h"
#include "MemoryHandler.h"

class GPUMemoryHandler: public MemoryHandler {
public:
  static GPUMemoryHandler * Get();
  virtual ~GPUMemoryHandler() { }
  virtual void * Allocate(unsigned n_bytes);
  virtual void Free(void * p);
  virtual void Zero(void * p, unsigned n_bytes);
  virtual void Copy(void * to, const void * from, unsigned n_bytes);
  unsigned CalcBlockDim(unsigned sz);
  unsigned NThr() { return nThr; }
  TFloat Reduce(TFloat * p_in, unsigned n);
  void ToGPU(void * p, void * p_d, unsigned nbytes);
  void FromGPU(void * p, void * p_d, unsigned nbytes);
protected:
  unsigned nThr;
  template<class T>
  T * _Allocate(unsigned n);
private:
  GPUMemoryHandler();
  GPUMemoryHandler(const GPUMemoryHandler &);
  GPUMemoryHandler & operator=(GPUMemoryHandler &);
};

inline unsigned GPUMemoryHandler::CalcBlockDim(unsigned sz) {
  return (sz-1)/nThr+1;
}

template<class T>
inline T * GPUMemoryHandler::_Allocate(unsigned n) {
  return reinterpret_cast<T*>(Allocate(n*sizeof(T)));
}
