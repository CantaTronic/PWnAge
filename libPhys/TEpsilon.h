
#ifndef TEpsilon_h
#define TEpsilon_h

#include "TFloat.h"

class TEpsilon {
  public:
    static const int permutationNumber = 24;
    static const int permutation012Number = 6;
    TEpsilon();
    int    permutations[permutationNumber][4];
    TFloat permutations_par[permutationNumber];
    int    permutations012[permutation012Number][4];
    TFloat permutations012_par[permutation012Number];
    inline void getPermutation(int n, TFloat & par, int & mu, int & nu, int & lam, int & rho) {
      mu  = permutations[n][0];
      nu  = permutations[n][1];
      lam = permutations[n][2];
      rho = permutations[n][3];
      par = permutations_par[n];
    }
    inline void getPermutation012(int n, TFloat & par, int & mu, int & nu, int & lam, int & rho) {
      mu  = permutations012[n][0];
      nu  = permutations012[n][1];
      lam = permutations012[n][2];
      rho = permutations012[n][3];
      par = permutations012_par[n];
    }
};
extern TEpsilon gEpsilon;

#endif
