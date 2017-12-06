
#include "multiindex.h"

void multiindex(long int mi, int l, int* ar) {
    for (int i=0; i<l; i++) {
        ar[i] = mi % 4;
        mi = mi / 4;
    }
}

TFloat metrics_product(int l, int* ar) {
    static TFloat g[4] = {-1., -1., -1., 1}; // move to globals
    TFloat t = 1.;
    for (int i=0; i<l; i++)
        t *= g[ar[i]]; //metrics is diagonal
    return t;
}
