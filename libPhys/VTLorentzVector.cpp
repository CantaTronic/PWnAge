// // @(#)root/physics:$Id$
// // Author: Pasha Murat , Peter Malzacher  12/02/99
// //    Oct  8 1999: changed Warning to Error and
// //                 return fX in TFloat & operator()
// //    Oct 20 1999: dito in TFloat operator()
// //    Jan 25 2000: implemented as (fP,fE) instead of (fX,fY,fZ,fE)
// short version for PWA framework -- modified by VT

#include "VTLorentzVector.h"

#include <iostream>

VTLorentzVector::VTLorentzVector()
               : fP(), fE() {}

VTLorentzVector::VTLorentzVector(TFloat x, TFloat y, TFloat z, TFloat t)
               : fP(x,y,z), fE(t) {}

VTLorentzVector::VTLorentzVector(const VTVector3 & p, TFloat e)
               : fP(p), fE(e) {}

VTLorentzVector::VTLorentzVector(const VTLorentzVector & p)
               : fP(p.Vect()), fE(p.T()) {}

VTLorentzVector::~VTLorentzVector()  {}

TFloat VTLorentzVector::operator () (int i) const
{
   //dereferencing operator const
   switch(i) {
      case kX:
      case kY:
      case kZ:
         return fP(i);
      case kT:
         return fE;
      default:
         std::cerr<<"VTLorentzVector::operator()(): bad index ("<<i<<") returning 0"<<std::endl;
   }
   return 0.;
}

TFloat & VTLorentzVector::operator () (int i)
{
   //dereferencing operator
   switch(i) {
      case kX:
      case kY:
      case kZ:
         return fP(i);
      case kT:
         return fE;
      default:
         std::cerr<<"VTLorentzVector::operator()(): bad index ("<<i<<") returning &fE"<<std::endl;
   }
   return fE;
}

void VTLorentzVector::Boost(TFloat bx, TFloat by, TFloat bz)
{
   //Boost this Lorentz vector
   TFloat b2 = bx*bx + by*by + bz*bz;
   register TFloat gamma = 1.0 / sqrt(1.0 - b2);
   register TFloat bp = bx*X() + by*Y() + bz*Z();
   register TFloat gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;

   SetX(X() + gamma2*bp*bx + gamma*bx*T());
   SetY(Y() + gamma2*bp*by + gamma*by*T());
   SetZ(Z() + gamma2*bp*bz + gamma*bz*T());
   SetT(gamma*(T() + bp));
}

//______________________________________________________________________________
void VTLorentzVector::Print() const
{
  // Print the TLorentz vector components as (x,y,z,t) and (P,eta,phi,E) representations
   std::cout<<"(x,y,z,t)=("<<fP.X()<<","<<fP.Y()<<","<<fP.Z()<<","<<fE<<")"<<std::endl;
}
