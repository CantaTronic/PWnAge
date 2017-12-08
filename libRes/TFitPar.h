
#pragma once

#include "TParArray.h"
#include "OffloadDirectives.h"  //for offload directives: PWA_START_OFFLOAD, PWA_END_OFFLOAD

PWA_START_OFFLOAD

class TFitPar {
  /*
   Класс отвечает за работу с одним параметром фита
   */
protected:
  static const std::string par_names[5];   //названия параметров
  TParArray * par;  //структура с массивами значений параметров
  unsigned offset;  //сдвиг (храним для кеширования)
public:
  TFitPar(TParArray * _par, unsigned _offset): par(_par), offset(_offset) { } //инициализация
  TFitPar(const TFitPar & x): par(x.par), offset(x.offset) { }
  TFitPar & operator= (const TFitPar & x);  //сравнение значений параметров 
  std::string Name();
  const char * CName() { return Name().c_str(); }
  void Set(TFloat v, TFloat step = 0.01); //установка значений параметров
  void Set(TFloat v, TFloat min, TFloat max, TFloat step = 0.01);
  void operator= (TFloat v) { par->SetParameter(offset, v); }
  operator TFloat() { return par->parameters[offset]; }
  TParType Type() const { return par->types[offset]; }
  bool Changed() { return par->types[offset] == kChanged; }
  void SetChanged() { if(par->types[offset] == kUnchanged) par->types[offset] = kChanged; }
  void SetUnchanged() { if(par->types[offset] == kChanged) par->types[offset] = kUnchanged; }
  bool IsFixed() { return par->types[offset] == kFixed; }
  TFloat Min() { return par->minima[offset]; }
  TFloat Max() { return par->maxima[offset]; }
  TFloat Step() { return par->steps[offset]; }
};

std::ostream & operator<< (std::ostream & os, TFitPar _p);

PWA_END_OFFLOAD
