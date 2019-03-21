# Par::Engine
Easy managed multithreading in C++! 
# Installation
Just drag `parengine.cpp` and `parengine.hpp` into your project, and compile them along with your other source.
On linux you have to link pthread (`-lpthread`), since this of course uses threading. 
# Examples
Here's an example,
```cpp
#include "parengine.hpp"

void countToTenMillion() {
  int i = 0;
  while(i < 10000000)
    i++;
}

int main() {
  Par::Engine engine(10); // 10 threads
  for (int i = 0; i < 10; i++)
    engine.PushWork(countToTenMillion);
  engine.Run();
  return 0;
}
```
This uses 10 threads to run 10 functions.
What if the function has parameters? Well, here's that,
```cpp
#include "parengine.hpp"
#include <atomic>
#include <iostream>

// Atomic counter to ensure thread safety
std::atomic<int> counter;

void addCustom(int custom) {
  counter += custom;
  std::cout << "Added custom!\n";
}

int main() {
  Par::Engine engine(10); // 10 threads
  for (int i = 0; i < 100; i++)
    engine.PushWork(std::bind(addCustom, i));
  engine.Run();
  return 0;
}
```
This uses 10 threads to run 100 functions, passing i to each function being run.
What if we want to run all functions with the same engine, but half the functions 
require the other half to be run before being run themselves? Here's that too,
```cpp
#include "parengine.hpp"
#include <atomic>

std::atomic<int> counter;

void addThree() {
  counter += 3;
}

void doubleCounter(){
  if(counter < 300) throw std::runtime_error("below 30"); 
  counter = counter * 2;
}

int main() {
  Par::Engine engine(10); // 10 threads
  for (int i = 0; i < 100; i++)
    engine.PushWork(addThree);
  engine.NewQueue(); // To create a new queue
  for (int i = 0; i < 100; i++)
    engine.PushWork(doubleCounter);
  engine.Run();
  return 0;
}
```
This uses 10 threads to run the `addThree` function a 100 times, and then it
uses 10 threads to run the `doubleCounter` function 100 times.
