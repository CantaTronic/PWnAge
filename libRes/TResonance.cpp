
#include "TResonance.h"
#include "TFitPar.h"
#include <iostream>

using std::endl;
using std::cerr;

const unsigned TResonance::nPar = 5;
const unsigned TResonance::nQN = 4;

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
