
#pragma once

// #include <vector>

class TStat {
private:
//   std::vector<double> dat;
  unsigned n;
  double sum, sumSq;
public:
  TStat(): n(0), sum(0), sumSq(0) {}
  void Clear() { n = 0; sum = 0; sumSq = 0; }
  void Add(double v);
  double Mean();
  double StDev();
};
