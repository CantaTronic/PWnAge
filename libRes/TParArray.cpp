
#include "TParArray.h"
#include "TResonance.h"
#include <iostream>

using std::cerr;    using std::endl;

void TParArray::UpdateL() {
  lmax = 0;
  for(unsigned i = 0; i < quantumNumbers.size(); i += TResonance::nQN)
    if(lmax < quantumNumbers[i])
      lmax = quantumNumbers[i];
}

void TParArray::UpdateNFreePar() {
  n_free_par = 0;
  for(unsigned i = 0; i < types.size(); i++)
    if(types[i] != kFixed)
      n_free_par++;
}

void TParArray::SetParameter(unsigned i, TFloat v) {
  if(types[i] == kFixed) {
    if(parameters[i] != v)
      cerr<<" Warning! TResonanceSet::SetParameters: Trying to change fixed parameter"<<endl;
  } else if(parameters[i] != v) {
    parameters[i] = v;
    types[i] = kChanged;
  } else {
    types[i] = kUnchanged;
  }
}

