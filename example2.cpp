#include "parengine.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono;

void countToTenMillion(){
  int i = 0;
  while(i < 10000000)
    i++;
}

// Example using 2 lines of code more than a serial solution
int main(){
  // Serial count to 100 million
  {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for (int i = 0; i < 10; i++)
      countToTenMillion();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    std::cout << "Serial time: " << duration << " ms\n";
  }
  
  // Parallel count to 100 million
  {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    Par::Engine engine(10); // 10 threads
    for (int i = 0; i < 10; i++)
      engine.PushWork(countToTenMillion);
    engine.Run();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    std::cout << "Parallel time: " << duration << " ms\n";
  }
  return 0;
}
