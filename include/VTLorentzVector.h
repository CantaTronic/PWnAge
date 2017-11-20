// @(#)root/physics:$Id$
// Author: Pasha Murat , Peter Malzacher  12/02/99

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef VTLorentzVector_h
#define VTLorentzVector_h

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// VTLorentzVector                                                      //
//                                                                      //
// Place holder for real lorentz vector class.                          //
//                                                                      //
// Short version by vt                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "VTVector3.h"

class VTLorentzVector {
private:
   VTVector3 fP;  // 3 vector component
   TFloat fE;  // time or energy of (x,y,z,t) or (px,py,pz,e)
   inline TFloat Max(TFloat a, TFloat b);
public:
   enum { kX=0, kY=1, kZ=2, kT=3, kNUM_COORDINATES=4, kSIZE=kNUM_COORDINATES };
   // Safe indexing of the coordinates when using with matrices, arrays, etc.

   //========конструкторы и деструктор======

   VTLorentzVector();

   VTLorentzVector(TFloat x, TFloat y, TFloat z, TFloat t);//!!!
   // Constructor giving the components x, y, z, t.

   VTLorentzVector(const VTVector3 & vector3, TFloat t);//!!!
   // Constructor giving a 3-Vector and a time component.

   VTLorentzVector(const VTLorentzVector & lorentzvector);
   // Copy constructor.

   virtual ~VTLorentzVector();
   // Destructor

   //=======операции обращения к компонентам вектора=============

   inline TFloat X() const;
   inline TFloat Y() const;
   inline TFloat Z() const;
   inline TFloat T() const;
   // Get position and time.

   inline void SetX(TFloat a);
   inline void SetY(TFloat a);
   inline void SetZ(TFloat a);
   inline void SetT(TFloat a);
   // Set position and time.

   inline TFloat Px() const;
   inline TFloat Py() const;
   inline TFloat Pz() const;
   inline TFloat P()  const;
   inline TFloat E()  const;
   inline TFloat Energy() const;
   // Get momentum and energy.

   inline void SetPx(TFloat a);
   inline void SetPy(TFloat a);
   inline void SetPz(TFloat a);
   inline void SetE(TFloat a);
   // Set momentum and energy.

   inline VTVector3 Vect() const ;
   // Get spatial component.
//
   inline void SetPxPyPzE(TFloat px, TFloat py, TFloat pz, TFloat e);
   inline void SetXYZT(TFloat  x, TFloat  y, TFloat  z, TFloat t);
   inline void SetXYZM(TFloat  x, TFloat  y, TFloat  z, TFloat m);

   TFloat operator () (int i) const;
   inline TFloat operator [] (int i) const;
   // Get components by index.

   TFloat & operator () (int i);
   inline TFloat & operator [] (int i);
   // Set components by index.

   inline VTLorentzVector & operator = (const VTLorentzVector &);
   // Assignment.

   inline VTLorentzVector   operator +  (const VTLorentzVector &) const;
   inline VTLorentzVector & operator += (const VTLorentzVector &);
   // Additions.

   inline VTLorentzVector   operator -  (const VTLorentzVector &) const;
   inline VTLorentzVector & operator -= (const VTLorentzVector &);
   // Subtractions.

   inline VTLorentzVector operator - () const;
   // Unary minus.

   inline VTLorentzVector operator * (TFloat a) const;
   inline VTLorentzVector & operator *= (TFloat a);
   // Scaling with real numbers.

   inline TFloat Mag2() const;
// 
   inline TFloat Dot(const VTLorentzVector &) const;
   inline TFloat operator * (const VTLorentzVector &) const;
   // Scalar product.

   inline TFloat Plus() const;
   inline TFloat Minus() const;
   // Returns t +/- z.
   // Related to the positive/negative light-cone component,
   // which some define this way and others define as (t +/- z)/sqrt(2)

   void Boost(TFloat, TFloat, TFloat);
   inline void Boost(const VTVector3 &);
   // Lorentz boost.

   virtual void        Print() const;
};

inline VTLorentzVector operator * (TFloat a, const VTLorentzVector &);
// Scaling LorentzVector with a real number

inline TFloat VTLorentzVector::X() const { return fP.X(); }
inline TFloat VTLorentzVector::Y() const { return fP.Y(); }
inline TFloat VTLorentzVector::Z() const { return fP.Z(); }
inline TFloat VTLorentzVector::T() const { return fE; }

inline void VTLorentzVector::SetX(TFloat a) { fP.SetX(a); }
inline void VTLorentzVector::SetY(TFloat a) { fP.SetY(a); }
inline void VTLorentzVector::SetZ(TFloat a) { fP.SetZ(a); }
inline void VTLorentzVector::SetT(TFloat a) { fE = a; }

inline TFloat VTLorentzVector::Px() const { return X(); }
inline TFloat VTLorentzVector::Py() const { return Y(); }
inline TFloat VTLorentzVector::Pz() const { return Z(); }
// inline TFloat VTLorentzVector::P()  const { return fP.Mag(); }
inline TFloat VTLorentzVector::E()  const { return T(); }
// inline TFloat VTLorentzVector::Energy()  const { return T(); }

inline void VTLorentzVector::SetPx(TFloat a) { SetX(a); }
inline void VTLorentzVector::SetPy(TFloat a) { SetY(a); }
inline void VTLorentzVector::SetPz(TFloat a) { SetZ(a); }
inline void VTLorentzVector::SetE(TFloat a)  { SetT(a); }

inline VTVector3 VTLorentzVector::Vect() const { return fP; }

inline void VTLorentzVector::SetXYZT(TFloat  x, TFloat  y, TFloat  z, TFloat t) {
   fP.SetXYZ(x, y, z);
   SetT(t);
}

inline void VTLorentzVector::SetPxPyPzE(TFloat px, TFloat py, TFloat pz, TFloat e) {
   SetXYZT(px, py, pz, e);
}

inline void VTLorentzVector::SetXYZM(TFloat  x, TFloat  y, TFloat  z, TFloat m) {
   if ( m  >= 0 ) 
//       SetXYZT( x, y, z, TMath::Sqrt(x*x+y*y+z*z+m*m) );
      SetXYZT( x, y, z, sqrt(x*x+y*y+z*z+m*m) );
   else 
//       SetXYZT( x, y, z, TMath::Sqrt( TMath::Max((x*x+y*y+z*z-m*m), 0. ) ) );
      SetXYZT( x, y, z, sqrt( Max((x*x+y*y+z*z-m*m), 0. ) ) );
}

TFloat & VTLorentzVector::operator [] (int i)       { return (*this)(i); }
TFloat   VTLorentzVector::operator [] (int i) const { return (*this)(i); }

inline VTLorentzVector &VTLorentzVector::operator = (const VTLorentzVector & q) { //!!!
   fP = q.Vect();
   fE = q.T();
   return *this;
}

inline VTLorentzVector VTLorentzVector::operator + (const VTLorentzVector & q) const {
   return VTLorentzVector(fP+q.Vect(), fE+q.T());
}

inline VTLorentzVector &VTLorentzVector::operator += (const VTLorentzVector & q) {
   fP += q.Vect();
   fE += q.T();
   return *this;
}

inline VTLorentzVector VTLorentzVector::operator - (const VTLorentzVector & q) const {
   return VTLorentzVector(fP-q.Vect(), fE-q.T());
}

inline VTLorentzVector &VTLorentzVector::operator -= (const VTLorentzVector & q) {
   fP -= q.Vect();
   fE -= q.T();
   return *this;
}

inline VTLorentzVector VTLorentzVector::operator - () const {
   return VTLorentzVector(-X(), -Y(), -Z(), -T());
}

inline VTLorentzVector& VTLorentzVector::operator *= (TFloat a) {
   fP *= a;
   fE *= a;
   return *this;
}

inline VTLorentzVector VTLorentzVector::operator * (TFloat a) const {
   return VTLorentzVector(a*X(), a*Y(), a*Z(), a*T());
}

inline TFloat VTLorentzVector::Mag2() const {
   return T()*T() - fP.Mag2();
}

inline TFloat VTLorentzVector::Dot(const VTLorentzVector & q) const {
   return T()*q.T() - Z()*q.Z() - Y()*q.Y() - X()*q.X();
}

inline TFloat VTLorentzVector::operator * (const VTLorentzVector & q) const {
   return Dot(q);
}

inline void VTLorentzVector::Boost(const VTVector3 & b) {
   Boost(b.X(), b.Y(), b.Z());
}

inline VTLorentzVector operator * (TFloat a, const VTLorentzVector & p) {
   return VTLorentzVector(a*p.X(), a*p.Y(), a*p.Z(), a*p.T());
}

inline TFloat VTLorentzVector::Max(TFloat a, TFloat b) {
  return a > b ? a : b;
}

#endif
