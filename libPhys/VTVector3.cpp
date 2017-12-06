// @(#)root/physics:$Id$
// Author: Pasha Murat, Peter Malzacher   12/02/99
//    Aug 11 1999: added Pt == 0 guard to Eta()
//    Oct  8 1999: changed Warning to Error and
//                 return fX in TFloat & operator()
//    Oct 20 1999: Bug fix: sign in PseudoRapidity
//                 Warning-> Error in TFloat operator()
// Short version by vt

#include "VTVector3.h"

#include <iostream>

#include <cmath>

//______________________________________________________________________________
VTVector3::VTVector3()
: fX(0.0), fY(0.0), fZ(0.0) {}

VTVector3::VTVector3(const VTVector3 & p)
: fX(p.fX), fY(p.fY), fZ(p.fZ) {}

VTVector3::VTVector3(TFloat xx, TFloat yy, TFloat zz)
: fX(xx), fY(yy), fZ(zz) {}

VTVector3::~VTVector3() {}

//______________________________________________________________________________
TFloat VTVector3::operator () (int i) const {
   //dereferencing operator const
   switch(i) {
      case 0:
         return fX;
      case 1:
         return fY;
      case 2:
         return fZ;
      default:
         std::cerr<<"VTVector3::operator()(i): bad index ("<<i<<") returning 0"<<std::endl;
   }
   return 0.;
}

//______________________________________________________________________________
TFloat & VTVector3::operator () (int i) {
   //dereferencing operator
   switch(i) {
      case 0:
         return fX;
      case 1:
         return fY;
      case 2:
         return fZ;
      default:
         std::cerr<<"VTVector3::operator()(i): bad index ("<<i<<") returning fX"<<std::endl;
   }
   return fX;
}

//______________________________________________________________________________
TFloat VTVector3::Mag() const
{
   // return the magnitude (rho in spherical coordinate system)
   return sqrt(Mag2());
}

VTVector3 operator + (const VTVector3 & a, const VTVector3 & b) {
   return VTVector3(a.X() + b.X(), a.Y() + b.Y(), a.Z() + b.Z());
}

VTVector3 operator - (const VTVector3 & a, const VTVector3 & b) {
   return VTVector3(a.X() - b.X(), a.Y() - b.Y(), a.Z() - b.Z());
}

VTVector3 operator * (const VTVector3 & p, TFloat a) {
   return VTVector3(a*p.X(), a*p.Y(), a*p.Z());
}

VTVector3 operator * (TFloat a, const VTVector3 & p) {
   return VTVector3(a*p.X(), a*p.Y(), a*p.Z());
}

TFloat operator * (const VTVector3 & a, const VTVector3 & b) {
   return a.Dot(b);
}

void VTVector3::Print()const
{
   //print vector parameters
     std::cout<<"(x,y,z)=("<<X()<<","<<Y()<<","<<Z()<<")"<<std::endl;
}
