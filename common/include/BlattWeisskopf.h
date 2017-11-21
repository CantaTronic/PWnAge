
#pragma once

#include "OffloadDirectives.h"
#include "TFloat.h"

PWA_OFFLOADED
inline TFloat pownd(TFloat x, int n) {
  if(n == 0) return 1;
  TFloat r = x;
  for(int i = 2; i <= n; i++) r *= x;
  return r;
}

PWA_OFFLOADED
inline TFloat BlattWeisskopfFact(int l, TFloat kTsq, TFloat r) {
  TFloat dx = 1.;
  TFloat r2 = pownd(r/0.1973, 2);
  TFloat z  = kTsq*r2;
  if(l == 1)
    dx = 1. + z;
  if(l == 2)
    dx = 9. + 3.*z + z*z;
  if(l == 3)
    dx = 225. + 45.*z + 6.*z*z + pownd(z, 3);
  if(l == 4)
    dx = 11025. + 1575.*z + 135.*z*z + 10.*pownd(z, 3) + pownd(z, 4);
  if(l == 5)
    dx = 893025. + 99225.*z + 6300.*z*z + 315.*pownd(z, 3) + 15.*pownd(z, 4) + pownd(z, 5);
  if(l == 6)
    dx = 108056025. + 9823275.*z + 496125.*z*z + 18900.*pownd(z, 3) + 630.*pownd(z, 4)
        + 21.*pownd(z,5) + pownd(z,6);
  if(l >= 1) dx = dx/pownd(r2, l);
//      if(l >= 7) dx=dx*((1.+4.*q2)*r2)**(L-6)
  return dx;
}

PWA_OFFLOADED
inline TFloat Rho(TFloat s, int J, TFloat qsq, TFloat r) {
  return 2 * sqrt(qsq/s) * pownd(qsq, J) / BlattWeisskopfFact(J, qsq, r);
}

PWA_OFFLOADED
inline TFloat q2(TFloat s, TFloat m1, TFloat m2) {
  return (s - (m1+m2)*(m1+m2)) * (s - (m1-m2)*(m1-m2)) / (4*s);
}

