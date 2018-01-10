
#include "SimpTimer.h"
#include <chrono>

void SimpTimer::start() {
  startTime = std::chrono::high_resolution_clock::now();
}

void SimpTimer::stop() {
  endTime = std::chrono::high_resolution_clock::now();
}

float SimpTimer::rate() {
  stop();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  return ((float) duration / (float) 1000);
}

