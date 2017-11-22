
#pragma once

#ifdef __INTEL_COMPILER
#include "OffloadDirectives.h"
#include "TCalcCPU.h"

class TCachePHI: public TCache {
public:
  TCachePHI(TCache * x, unsigned offset = 0, unsigned _nev = 0);
  virtual ~TCachePHI() { }
  virtual void Allocate() { }
  virtual const char * Buffer() { return buffer.data(); }
  virtual unsigned BufSize() { return buffer.size(); }
protected:
  string buffer;
private:
  TCachePHI & operator= (const TCachePHI & x); // copy-assignment
};

PWA_START_OFFLOAD
class TCalcPHI: public TCalcCPU {
public:
  TCalcPHI(unsigned sz, const char * buf);
  virtual ~TCalcPHI();
protected:
private:
  TCalcPHI(const TCalcPHI & x);              // copy-constructor
  TCalcPHI & operator= (const TCalcPHI & x); // copy-assignment
};
PWA_END_OFFLOAD

class PHIMemoryHandler: public MemoryHandler {
public:
  static PHIMemoryHandler * Get();
  virtual ~PHIMemoryHandler() { }
  virtual void * Allocate(unsigned n_bytes);
  virtual void Free(void * p);
  virtual void Zero(void * p, unsigned n_bytes);
  virtual void Copy(void * to, const void * from, unsigned n_bytes);
  void ToPHI(void * p, void * p_d, unsigned n_bytes);
private:
  PHIMemoryHandler() { }
  PHIMemoryHandler(const PHIMemoryHandler &);
  PHIMemoryHandler & operator=(PHIMemoryHandler &);
};
#endif
