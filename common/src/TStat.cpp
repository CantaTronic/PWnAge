
#include "TStat.h"
#include <cmath>

void TStat::Add(double v) {
  n++;
  sum += v;
  sumSq += v*v;
}

double TStat::Mean() {
  if(!n) return 0;
  return sum/n;
}

double TStat::StDev() {
  if(n <= 1) return 0;
  double m = Mean();
  return sqrt((sumSq - 2*m*sum + m*m*n)/(n-1));
}
