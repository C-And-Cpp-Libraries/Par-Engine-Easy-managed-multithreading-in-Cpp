#include "parengine.hpp"
// #include <thread> // Optional: For system thread count
#include <atomic>
#include <iostream>

// Atomic counter to ensure thread safety
std::atomic<int> counter;

// Adds 1 to our counter
void addOne() {
  counter++;
  std::cout << "Added one!\n";
}

// Adds an arbitrary int to our counter
void addCustom(int custom) {
  counter += custom;
  std::cout << "Added custom!\n";
}

// This function requires both addition functions to
// be run before execution. Here, we need to add this to
// a queue that runs AFTER addOne() and addCustom();
void doubleCounter(){
  // Fake dependancy :) Exceptions are printed with e.what()
  if(counter < 30) throw std::runtime_error("below 30"); 
  counter = counter * 2;
  std::cout << "Doubled!\n";
}

int main() {
  //  Create a Par::Engine
  //  By default it will use 4 threads.
  //  If you already have "#include <thread>" in your project, you can get
  //  the system thread count with "std::thread::hardware_concurrency()".
  // Par::Engine engine(); // This uses 4 threads.
  // Par::Engine engine(std::thread::hardware_concurrency()); // System thread count
  Par::Engine engine(8); // This uses 8 threads.
  
  // We want to run addOne() and addCustom(2) 10 times, 
  // so we get enough for the doubleCounter().
  for(int i = 0; i < 10; i++) {
    engine.PushWork(addOne);
    // addCustom(int custom) requires an argument so we bind it.
    engine.PushWork(std::bind(addCustom, 2));
  }
  
  // Now to call doubleCounter() we have to make sure it's in a seperate queue
  // Engine::NewQueue() indicates that we want to create a new queue.
  engine.NewQueue();
  // Now we can run doubleCounter() 10 times.
  for(int i = 0; i < 10; i++) {
    engine.PushWork(doubleCounter);
  }
  // To run our queues we use Engine::Run(). This also clears the queue, 
  // allowing for reuse of the same object.
  std::cout << "counter : " << counter << std::endl;
  engine.Run();
  std::cout << "counter : " << counter << std::endl;
  
  return 0;
}




