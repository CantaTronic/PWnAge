
#include "SimpTimer.h"
#include <chrono>

// using std::chrono;


void SimpTimer::start() {
  startTime = std::chrono::high_resolution_clock::now();
}

void SimpTimer::stop() {
  endTime = std::chrono::high_resolution_clock::now();
}

//   float SimpTimer::spotRate(size_t eventsThusFar) {
//     std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
//     return (float) eventsThusFar / ((float) duration / (float) 1000);
//   }

float SimpTimer::rate() {
  stop();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  return ((float) duration / (float) 1000);
}

