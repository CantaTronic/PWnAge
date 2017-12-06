
#include "TEpsilon.h"

TEpsilon gEpsilon;

int calc_eps(int mu, int nu, int lam, int rho) {
  // needs testing
  int ar[4] = {mu, nu, lam, rho};
  int perm = 0;
  // sort array
  for (int i=0; i<4-1; i++)
    for (int j=0; j<4-1-i; j++) {
    if (ar[j]==ar[j+1]) return 0;
    if (ar[j] > ar[j+1]) {
      int t = ar[j+1];
      ar[j+1] = ar[j];
      ar[j] = t;
      perm++;
    }
    }
    if ((perm % 2) == 0) return 1;
    else return -1;
}

TEpsilon::TEpsilon() {
  int perm_n=0;
  for (int mu=0; mu<4; mu++)
    for (int nu=0; nu<4; nu++)
      for (int lam=0; lam<4; lam++)
        for (int rho=0; rho<4; rho++) {
          int par = calc_eps(mu, nu, lam, rho);
          if (par != 0) {
            permutations[perm_n][0] = mu;
            permutations[perm_n][1] = nu;
            permutations[perm_n][2] = lam;
            permutations[perm_n][3] = rho;
            permutations_par[perm_n] = par;
            perm_n++;
          }
        }
        int perm012_n = 0;
        for (int mu=0; mu<3; mu++)
          for (int nu=0; nu<3; nu++)
            for (int lam=0; lam<3; lam++) {
              int par = calc_eps(mu, nu, lam, 3);
              if (par != 0) {
                permutations012[perm012_n][0] = mu;
                permutations012[perm012_n][1] = nu;
                permutations012[perm012_n][2] = lam;
                permutations012[perm012_n][3] = 3;
                permutations012_par[perm012_n] = par;
                perm012_n++;
              }
            }
}
