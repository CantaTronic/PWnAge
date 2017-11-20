
#ifndef TVertex1to2_h
#define TVertex1to2_h

#include "TX.h"

struct TVertex1to2 {
    int lmax;
    VTLorentzVector P;
    VTLorentzVector kT;
    TFloat s;
    TFloat gT[4][4];
    TFloat gT_ui[4][4]; //upper indexes
    TVertex1to2(int lmax, VTLorentzVector p1, VTLorentzVector p2);
    ~TVertex1to2();
    // building blocks
    TX ** X;
};

#endif
