// @(#)root/physics:$Id$
// Author: Pasha Murat, Peter Malzacher   12/02/99

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
// Short version by vt
#pragma once

#include "TFloat.h"

class VTVector3 {

public:

   VTVector3();

   VTVector3(TFloat x, TFloat y, TFloat z);
   // The constructor.

   VTVector3(const VTVector3 &);
   // The copy constructor.

   virtual ~VTVector3();
   // Destructor

   TFloat operator () (int) const;
   inline TFloat operator [] (int) const;
   // Get components by index (Geant4).

   TFloat & operator () (int);
   inline TFloat & operator [] (int);
   // Set components by index.

   inline TFloat X()  const;
   inline TFloat Y()  const;
   inline TFloat Z()  const;

   inline void SetX(TFloat);
   inline void SetY(TFloat);
   inline void SetZ(TFloat);
   inline void SetXYZ(TFloat x, TFloat y, TFloat z);

   inline TFloat Mag2() const;
   // The magnitude squared (rho^2 in spherical coordinate system).

   TFloat Mag() const;
   // The magnitude (rho in spherical coordinate system).

   inline VTVector3 & operator = (const VTVector3 &);
   // Assignment.

   inline bool operator == (const VTVector3 &) const;
   inline bool operator != (const VTVector3 &) const;
   // Comparisons (Geant4).

   inline VTVector3 & operator += (const VTVector3 &);
   // Addition.

   inline VTVector3 & operator -= (const VTVector3 &);
   // Subtraction.

   inline VTVector3 operator - () const;
   // Unary minus.

   inline VTVector3 & operator *= (TFloat);
   // Scaling with real numbers.

   inline TFloat Dot(const VTVector3 &) const;
   // Scalar product.

   void Print() const;

private:

   TFloat fX, fY, fZ;
   // The components.

};

VTVector3 operator + (const VTVector3 &, const VTVector3 &);
// Addition of 3-vectors.

VTVector3 operator - (const VTVector3 &, const VTVector3 &);
// Subtraction of 3-vectors.

TFloat operator * (const VTVector3 &, const VTVector3 &);
// Scalar product of 3-vectors.

VTVector3 operator * (const VTVector3 &, TFloat a);
VTVector3 operator * (TFloat a, const VTVector3 &);
// Scaling of 3-vectors with a real number

TFloat & VTVector3::operator[] (int i)       { return operator()(i); }
TFloat   VTVector3::operator[] (int i) const { return operator()(i); }

inline TFloat VTVector3::X()  const { return fX; }
inline TFloat VTVector3::Y()  const { return fY; }
inline TFloat VTVector3::Z()  const { return fZ; }

inline void VTVector3::SetX(TFloat xx) { fX = xx; }
inline void VTVector3::SetY(TFloat yy) { fY = yy; }
inline void VTVector3::SetZ(TFloat zz) { fZ = zz; }

inline void VTVector3::SetXYZ(TFloat xx, TFloat yy, TFloat zz) {
   fX = xx;
   fY = yy;
   fZ = zz;
}

inline VTVector3 & VTVector3::operator = (const VTVector3 & p) {
   fX = p.fX;
   fY = p.fY;
   fZ = p.fZ;
   return *this;
}

inline bool VTVector3::operator == (const VTVector3& v) const {
   return (v.fX==fX && v.fY==fY && v.fZ==fZ) ? true : false;
}

inline bool VTVector3::operator != (const VTVector3& v) const {
   return (v.fX!=fX || v.fY!=fY || v.fZ!=fZ) ? true : false;
}

inline VTVector3& VTVector3::operator += (const VTVector3 & p) {
   fX += p.fX;
   fY += p.fY;
   fZ += p.fZ;
   return *this;
}

inline VTVector3& VTVector3::operator -= (const VTVector3 & p) {
   fX -= p.fX;
   fY -= p.fY;
   fZ -= p.fZ;
   return *this;
}

inline VTVector3 VTVector3::operator - () const {
   return VTVector3(-fX, -fY, -fZ);
}

inline VTVector3& VTVector3::operator *= (TFloat a) {
   fX *= a;
   fY *= a;
   fZ *= a;
   return *this;
}

inline TFloat VTVector3::Dot(const VTVector3 & p) const {
   return fX*p.fX + fY*p.fY + fZ*p.fZ;
}

inline TFloat VTVector3::Mag2() const { return fX*fX + fY*fY + fZ*fZ; }
