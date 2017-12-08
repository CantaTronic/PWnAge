
#include "TResonanceSet.h"
#include "TCacheFile.h"
#include "TGlobals.h"
using std::cerr;
using std::endl;

#ifdef __INTEL_COMPILER
  #pragma offload_attribute(push, target(mic))
#endif
#include <iomanip>
#ifdef __INTEL_COMPILER
  #pragma offload_attribute(pop)
#endif
using std::setw;

const unsigned TResonance::nPar = 5;
const unsigned TResonance::nQN = 4;
const string TFitPar::par_names[5] = {"M", "G", "r", "Co", "Ph"};

string TFitPar::Name() {
  return par->names[offset/TResonance::nPar]+"_"+par_names[offset%TResonance::nPar];
}

TFitPar & TFitPar::operator= (const TFitPar & x) {
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

TResonance & TResonance::operator= (const TResonance & x) {
  if(this == &x) return *this;
  par = x.par;
  offsetPar = x.offsetPar;
  offsetQN = x.offsetQN;
  offsetSM = x.offsetSM;
  return *this;
}

TFitPar TResonance::Par(unsigned i) {
  if(i >= nPar) {
    cerr<<" Warning! TResonance::Par: No parameter #"<<i<<" in resonance"<<endl;
    i = nPar-1;
  }
  return TFitPar(par, offsetPar+i);
}

TResonanceSet & TResonanceSet::operator= (const TResonanceSet & x) {
  if(this == &x) return *this;
  TParArray::operator= (x);
  return *this;
}

TResonance TResonanceSet::Add(string name) {
  names.push_back(name);
  unsigned n_res = names.size();
  unsigned new_n_par = n_res*TResonance::nPar;
  parameters.resize(new_n_par);
  types.resize(new_n_par);
  minima.resize(new_n_par);
  maxima.resize(new_n_par);
  steps.resize(new_n_par);
  quantumNumbers.resize(n_res*TResonance::nQN);
  symmetryMultipliers.resize(n_res*3);
  TResonance res = Res(n_res-1);
  res.Par(0).Set(1.0,  0.1, 3.0);
  res.Par(1).Set(0.2, 0.01, 1.0);
  res.Par(2).Set(0.73, 0.200, 1.0);
  res.Par(3).Set(1, -100, 100);
  res.Par(4).Set(0, -2*_pi, 2*_pi);
  res.J(1);
  res.P(0);
  res.C(0);
  res.type(0);
  res.SymmetryMultiplier(0) =  1;
  res.SymmetryMultiplier(1) = -1;
  res.SymmetryMultiplier(2) =  0;
  return res;
}

TResonance TResonanceSet::Res(unsigned i) {
  if(i >= N()) {
    cerr<<" Warning! TResonanceSet::Res: No resonance #"<<i<<" in set"<<endl;
    i = N()-1;
  }
  return TResonance(this, i);
}

TFitPar TResonanceSet::Par(unsigned i) {
  if(i >= NPar()) {
    cerr<<" Warning! TResonanceSet::Par: No parameter #"<<i<<" in resonance"<<endl;
    i = NPar()-1;
  }
  return TFitPar(this, i);
}

void TResonanceSet::Clear() {
  names.clear();
  parameters.clear();
  types.clear();
  minima.clear();
  maxima.clear();
  steps.clear();
  quantumNumbers.clear();
  symmetryMultipliers.clear();
  lmax = 0;
  n_free_par = 0;
}

void TResonanceSet::SetParameters(double * par) {
  for(unsigned i = 0; i < parameters.size(); i++)
    SetParameter(i, par[i]);
}

void TResonanceSet::GetParameters(double * par) {
  for(unsigned i = 0; i < parameters.size(); i++)
    par[i] = parameters[i];
}

void TResonanceSet::Changed() {
  for(unsigned i = 0; i < types.size(); i++)
    if(types[i] == kUnchanged)
      types[i] = kChanged;
}

void TResonanceSet::PrintParameters() {
  for(unsigned i = 0; i < NPar(); i++)
    cerr<<setw(3)<<i<<Par(i)<<endl;
}

void TResonanceSet::PrintChanged() {
  for(unsigned i = 0; i < NPar(); i++)
    if(Par(i).Changed())
      cerr<<setw(3)<<i<<Par(i)<<endl;
}

void TResonanceSet::Write(OCacheFile * cf) {
  unsigned nres = N();
  cf->Write(nres);
  for(unsigned i = 0; i < nres; i++)
    cf->Write(names[i]);
  cf->Write(parameters);
  cf->Write(types);
  cf->Write(minima);
  cf->Write(maxima);
  cf->Write(steps);
  cf->Write(quantumNumbers);
  cf->Write(symmetryMultipliers);
}

void TResonanceSet::Read(ICacheFile * cf) {
  unsigned nres = 0, npar = 0;
  cf->Read(nres);
  npar = nres*TResonance::nPar;
  names.resize(nres);
  parameters.resize(npar);
  types.resize(npar);
  minima.resize(npar);
  maxima.resize(npar);
  steps.resize(npar);
  quantumNumbers.resize(nres*TResonance::nQN);
  symmetryMultipliers.resize(nres*3);
  for(unsigned i = 0; i < nres; i++)
    cf->Read(names[i]);
  cf->Read(parameters);
  cf->Read(types);
  cf->Read(minima);
  cf->Read(maxima);
  cf->Read(steps);
  cf->Read(quantumNumbers);
  cf->Read(symmetryMultipliers);
  UpdateL();
  UpdateNFreePar();
}

bool TResonanceSet::operator== (const TResonanceSet & x) {
  if(this == &x) return true;
  bool is_equal =
    ( names == x.names &&
      parameters == x.parameters &&
      types == x.types &&
      minima == x.minima &&
      maxima == x.maxima &&
      steps == x.steps &&
      quantumNumbers == x.quantumNumbers &&
      symmetryMultipliers == x.symmetryMultipliers );
  return is_equal;
}

bool TResonanceSet::operator!= (const TResonanceSet & x) {
  return !((*this) == x);
}
