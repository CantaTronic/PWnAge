
#ifndef TParticle_h
#define TParticle_h

#include "VTLorentzVector.h"

// struct TParticle {
//   int pdg_code;
//   VTLorentzVector p;
  // something may be added to account for polarization in derived class
// };
struct TEvent {
  // supposed to be maximally general form
  int number;
  TFloat weight;
//   TParticle particles[3]; //may need to be reworked
  VTLorentzVector particles[3]; //may need to be reworked
};

#endif
