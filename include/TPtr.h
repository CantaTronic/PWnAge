
#pragma once

#include "OffloadDirectives.h"
#include "MemoryHandler.h"

PWA_START_OFFLOAD
#include <vector>
PWA_END_OFFLOAD
using std::vector;

PWA_START_OFFLOAD
class CPUMemoryHandler: public MemoryHandler {
public:
  static CPUMemoryHandler * Get();
  virtual ~CPUMemoryHandler() { }
  virtual void * Allocate(unsigned n_bytes);
  virtual void Free(void * p);
  virtual void Zero(void * p, unsigned n_bytes);
  virtual void Copy(void * to, const void * from, unsigned n_bytes);
private:
  CPUMemoryHandler() { }
  CPUMemoryHandler(const CPUMemoryHandler &);
  CPUMemoryHandler & operator=(CPUMemoryHandler &);
};

template<class T>
struct TPtr {
  unsigned n;
  T * p, * o;
  MemoryHandler * memHandler;
  TPtr(): n(0), p(0), o(0), memHandler(CPUMemoryHandler::Get()) { }
  TPtr(MemoryHandler * h): n(0), p(0), o(0), memHandler(h) { }
  TPtr(const TPtr & x): n(x.n), p(x.p), o(x.o), memHandler(x.memHandler) { }
  ~TPtr() { }
  TPtr & operator=(const TPtr & x);
  void SetMemHandler(MemoryHandler * h) { memHandler = h; }
  void Shift(unsigned offset, unsigned _n) { if(p) { p += offset; n= _n; } }
  unsigned Allocate(unsigned n_ele);
  void Cleanup() { if(p) { memHandler->Free(p); p = 0; } }
  void Zero() { memHandler->Zero(p, n*sizeof(T)); }
  void FillWith(TPtr<T> & x) { memHandler->Copy(p, x.p, n*sizeof(T)); }
  unsigned AllocateCopy() { if(!n) return 0; o = p; p = 0; return Allocate(n); }
  void DropCopy() { Cleanup(); p = o; o = 0; }
  operator bool() { return p; }
  operator T*() { return p; }
  inline T & operator[] (unsigned i) { return p[i]; }
};

template<class T>
struct TPtrVec: public vector< TPtr<T> > {
  TPtrVec(): vector< TPtr<T> >() { }
  TPtrVec(unsigned sz): vector< TPtr<T> >(sz) { }
  TPtrVec(unsigned sz, MemoryHandler * h);
  void SetMemHandler(MemoryHandler * h)
      { for(unsigned i = 0; i < this->size(); i++) this->at(i).memHandler = h; }
  void Shift(unsigned offset, unsigned _n);
  unsigned Allocate(unsigned n);
  void Cleanup();
  void DropCopy() { for(unsigned i = 0; i < this->size(); i++) if(this->at(i).o) this->at(i).DropCopy(); }
};
PWA_END_OFFLOAD

template<class T>
TPtr<T> & TPtr<T>::operator=(const TPtr<T> & x) {
  if(this == &x) return *this;
  n = x.n;
  p = x.p;
  o = x.o;
  memHandler = x.memHandler;
  return *this;
}

template<class T>
TPtrVec<T>::TPtrVec(unsigned sz, MemoryHandler * h)
: vector< TPtr<T> >(sz) {
  for(unsigned i = 0; i < this->size(); i++)
    this->at(i).SetMemHandler(h);
}

template<class T>
void TPtrVec<T>::Shift(unsigned offset, unsigned _n) {
  for(unsigned i = 0; i < this->size(); i++)
    this->at(i).Shift(offset, _n);
}

template<class T>
unsigned TPtrVec<T>::Allocate(unsigned n) {
  unsigned nbytes = 0;
  for(unsigned i = 0; i < this->size(); i++)
    nbytes += this->at(i).Allocate(n);
  return nbytes;
}

template<class T>
void TPtrVec<T>::Cleanup() {
  for(unsigned i = 0; i < this->size(); i++)
    this->at(i).Cleanup();
}

template<class T>
unsigned TPtr<T>::Allocate(unsigned n_ele) {
   if(p) memHandler->Free(p);
   n = n_ele;
   unsigned sz = n*sizeof(T);
   p = reinterpret_cast<T*>(memHandler->Allocate(sz));
   return sz;
}
