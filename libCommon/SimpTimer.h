
#ifndef SimpTimer_h
#define SimpTimer_h
#include <chrono>

class SimpTimer {
public:
  SimpTimer(){
    start();
  }

  void start();

  void stop();

//   float spotRate(size_t eventsThusFar);
  float rate();

private:
  size_t nevents;
  std::chrono::high_resolution_clock::time_point startTime;
  std::chrono::high_resolution_clock::time_point endTime;
};


#endif
