
#include "TFitPar.h"
#include "TResonance.h"
#include <iomanip>
#include <iostream>

using std::setw;

const string TFitPar::par_names[5] = {"M", "G", "r", "Co", "Ph"}; //all possible paramaters names

string TFitPar::Name() {
  /**/
  return par->names[offset/TResonance::nPar]+"_"+par_names[offset%TResonance::nPar];
}

TFitPar & TFitPar::operator= (const TFitPar & x) {
  /*Assign TFitPar object on the left side to given TFitPar x*/
  if(this == &x) return *this;
  par = x.par;
  offset = x.offset;
  return *this;
}

void TFitPar::Set(TFloat v, TFloat step) {
  par->parameters[offset] = v;
  par->types[offset] = step?kChanged:kFixed;
  par->UpdateNFreePar();
  par->minima[offset] = 0;
  par->maxima[offset] = 0;
  par->steps[offset] = step;
}

void TFitPar::Set(TFloat v, TFloat min, TFloat max, TFloat step) {
  par->parameters[offset] = v;
  par->types[offset] = kChanged;
  par->UpdateNFreePar();
  par->minima[offset] = min;
  par->maxima[offset] = max;
  par->steps[offset] = step;
}

std::ostream & operator<< (std::ostream & os, TFitPar _p) {
  os<<setw(15)<<_p.Name()<<":"
    <<setw(10)<<static_cast<TFloat>(_p)
    <<" ["<<setw(10)<<_p.Min()<<","<<setw(10)<<_p.Max()<<"] ("<<_p.Changed()<<")";
  return os;
}
