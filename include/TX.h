
#ifndef TX_h
#define TX_h

#include "VTLorentzVector.h"

struct TX {
    int l;    // rank
    TFloat * val; // values
    // interface
    TFloat get(int *ind_ar); // array of four-dimensinal indexes
    TFloat get0123(int n0, int n1, int n2, int n3); // array of four-dimensinal indexes
    void   set0123(int n0, int n1, int n2, int n3, TFloat x);

    TX(int l, VTLorentzVector &kT, TFloat gT_ui[4][4], TX *, TX *);
    ~TX();
};

inline int pown(int x, int n) {
  if (n==0) return 1;
  int r = x;
  for (int i=2; i<=n; i++) r *= x;
  return r;
}

#endif
