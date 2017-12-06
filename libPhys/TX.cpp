
#include "TX.h"

#include <iostream>
#include <cstdio>
#include <cstdlib> 

using std::cout;
using std::cerr;
using std::endl;

TFloat TX::get0123(int n0, int n1, int n2, int n3) {// inline
    int ind = n0 + n1*(l+1) + n2*pown((l+1),2) + n3*pown((l+1),3);
    return val[ind]; // inline
}

void TX::set0123(int n0, int n1, int n2, int n3, TFloat x) {// inline
    int ind = n0 + n1*(l+1) + n2*pown((l+1),2) + n3*pown((l+1),3);
    if (ind > l*(l+1)*(l+1)*(l+1) ) {
        cerr << "Fatal error in TX::set0123" << endl;
        exit(1);
    }
#ifdef DEBUG_TX
    cout << "Setting " << n0 << n1 << n2 << n3 << " to " << x << endl;
#endif
    val[ind] = x; 
}

TFloat TX::get(int *ind_ar) {
    int n0 = 0;
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    for (int i=0; i<l; i++) {
        if (ind_ar[i] == 0) n0++;
        if (ind_ar[i] == 1) n1++;
        if (ind_ar[i] == 2) n2++;
        if (ind_ar[i] == 3) n3++;
    }
    return get0123(n0, n1, n2, n3);
}

int remove_ind0123(int i, int &n0, int &n1, int &n2, int &n3) {
    // returns 0, 1, 2 or depending on which index was excluded
    if (i<1) {
        printf("Wrond index value\n");
        exit(1);
    }

    if (i<=n0) {
        n0--;
        return 0;
    }
    if ((i > n0) && (i<=n0+n1)){
        n1--;
        return 1;
    }
    if ((i > n0+n1) && (i<=n0+n1+n2)) {
        n2--;
        return 2;
    }
//     if (i > n0+n1+n2) {
        n3--;
        return 3;
//     }
}



TX::TX(int lX, VTLorentzVector &kT, TFloat gT_ui[4][4], TX *Xm1, TX *Xm2) {
    l = lX;
#ifdef DEBUG_TX
    cout << "In TX constructor: l=" << l << endl;
#endif
    if (l==0) {
        val = new TFloat[1];
        val[0] = 1.;
        return;
    }

    val = new TFloat [l*(l+1)*(l+1)*(l+1)+1];
    if (l==1) {
        //val = new TFloat[4];
        set0123(1, 0, 0, 0, kT[0]);
        set0123(0, 1, 0, 0, kT[1]);
        set0123(0, 0, 1, 0, kT[2]);
        set0123(0, 0, 0, 1, kT[3]);
        //for (int mu=0; mu<4; mu++)
        //    val[mu] = kT[mu]; //check ordering!
        return;
    }

    // if l>=2 the recurrent expression is applied
    // 0000000011111222223
    for (int n0=0; n0<=l; n0++)
        for (int n1=0; n1+n0<=l; n1++)
            for (int n2=0; n0+n1+n2<=l; n2++) {
                int n3 = l - n0 - n1 - n2;
                if (n3 < 0) continue;

                // the first term of the sum
                TFloat t1=0;
                for (int i=1; i<=l; i++) {
                    // the first term of sum
                    int tn0 = n0;
                    int tn1 = n1;
                    int tn2 = n2;
                    int tn3 = n3;
                    int ikT = remove_ind0123(i, tn0, tn1, tn2, tn3);
#ifdef DEBUG_TX
                    printf("i= %d, %d %d %d %d\n", i, n0, n1, n2, n3);
                    printf("i= %d, %d %d %d %d ikT=%d\n", i, tn0, tn1, tn2, tn3, ikT);
#endif
                    t1 += Xm1->get0123(tn0, tn1, tn2, tn3) * kT[ikT];
                }
                t1 *= (TFloat(2)*l-1)/l/l;

                // the second term of the sum
                TFloat t2 = 0.;
                for (int i=1; i<=l; i++)
                    for (int j=i+1; j<=l; j++) {
                        int tn0 = n0;
                        int tn1 = n1;
                        int tn2 = n2;
                        int tn3 = n3;
                        int igT1 = remove_ind0123(i,   tn0, tn1, tn2, tn3);
                        int igT2 = remove_ind0123(j-1, tn0, tn1, tn2, tn3);
#ifdef DEBUG_TX
                        printf("(i,j)=(%d,%d), %d %d %d %d\n",        i, j, n0, n1, n2, n3);
                        printf("(i,j)=(%d,%d), %d %d %d %d igT1=%d igT2=%d\n", i, j, tn0, tn1, tn2, tn3, igT1, igT2);
                        printf("gT[igT1][igT2] = %f, Xm2->get0123(tn0, tn1, tn2, tn3)= %f\n", gT_ui[igT1][igT2], Xm2->get0123(tn0, tn1, tn2, tn3));
#endif
                        t2 += gT_ui[igT1][igT2] * Xm2->get0123(tn0, tn1, tn2, tn3);
                    }
                t2 *= 2.*kT.Mag2() /l/l;
#ifdef DEBUG_TX
                cout << "t1-t2: " << t1 << "+" << t2 << endl;
#endif
                set0123(n0, n1, n2, n3, t1-t2);
            }
}

TX::~TX() {
    delete [] val;
}
