#include "parengine.hpp"
#include <iostream>
#include <thread>

template class std::function<void(void)>;
namespace Par {  
  unsigned int GetCPUCount(){
    return std::thread::hardware_concurrency();
  }
  
  // Constructors
  Engine::Engine() : queuestarted(false), threadcount(GetCPUCount()), workqueues() {}
  Engine::Engine(size_t threads) : queuestarted(false), threadcount(threads), workqueues() {}
  
  // Make sure that queues are deleted
  Engine::~Engine(){ for(auto queue : workqueues) delete queue; }
  
  // Run queues in workqueues
  void Engine::Run(){
    std::thread* threads[threadcount];
    while(workqueues.size()) {
      for(size_t i = 0; i < threadcount; i++)
	threads[i] = new std::thread(&Engine::run, this, 
				     workqueues.front(), i);
      for(size_t i = 0; i < threadcount; i++) {  
	threads[i]->join();
	delete threads[i];
      }
      delete workqueues.front();
      workqueues.pop_front();
    }
  }
  
  // Addition of work
  void Engine::PushWork(Work work) { 
    if(!queuestarted)
      workqueues.push_back(new WorkQueue(threadcount));
    workqueues[workqueues.size()-1]->push_back(work);
    queuestarted = true;
  }
  
  // Function run in each thread
  void Engine::run(WorkQueue* queue, size_t index){
    while(queue->size(index)){
      try { queue->front(index)(); } 
      catch (const std::exception& e) { std::cout << e.what() << std::endl; }
      queue->pop(index);
    }
  }  
} // namespace Par
