
#include "TVertex1to2.h"
#include "TX.h"

#include <iostream>
using namespace std;

TVertex1to2::TVertex1to2(int l, VTLorentzVector p1, VTLorentzVector p2) {
    lmax = l;
#ifdef DEBUG_TVERTEX1TO2
    cout << "In TVertex1to2 constructor" << endl;
    p1.Print();
    p2.Print();
    cout << "lmax = " << lmax << endl;
#endif

    P = p1+p2;

    s = P.Mag2();

    // set gT
    TFloat g[4][4];
    for (int mu=0; mu<4; mu++)
        for (int nu=0; nu<4; nu++)
            g[mu][nu] = 0.;
    g[0][0] = -1.; // ROOT notations
    g[1][1] = -1.;
    g[2][2] = -1.;
    g[3][3] =  1.;

    for (int mu=0; mu<4; mu++)
        for (int nu=0; nu<4; nu++)
            gT[mu][nu] = g[mu][nu] - P[mu]*g[mu][mu]*P[nu]*g[nu][nu]/s;

    // rising index
    for (int mu=0; mu<4; mu++)
        for (int nu=0; nu<4; nu++)
            gT_ui[mu][nu] = g[mu][mu] * gT[mu][nu] * g[nu][nu];

    // set kT
    VTLorentzVector k = 0.5 * (p1 - p2);
    for (int mu=0; mu<4; mu++) {
        kT[mu] = 0.;
        for (int nu=0; nu<4; nu++)
            kT[mu] += g[mu][mu] * gT[mu][nu] * k[nu];
    }

    // generate tensors up to rank lmax
    // move to seperate function
    X = new TX* [lmax+1];
#ifdef DEBUG_TVERTEX1TO2
    cout << "lmax=" << lmax << endl;
#endif
    if (lmax>=0) X[0] = new TX(0, kT, gT_ui, 0, 0);
    if (lmax>=1) X[1] = new TX(1, kT, gT_ui, 0, 0);
    for (int i=2; i<=lmax; i++) {
        X[i] = new TX(i, kT, gT_ui, X[i-1], X[i-2]);
    }
}

TVertex1to2::~TVertex1to2() {
    for (int i=0; i<=lmax; i++) delete X[i];
    delete [] X;
}
