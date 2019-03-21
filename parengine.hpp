#ifndef PARENGINE_HPP
#define PARENGINE_HPP
#include <deque>
#include <functional>

///////////////////////////////////////////////////
// Alternative (slower) push_back implementation //
///////////////////////////////////////////////////
/*void push_back(const T &element) {
  size_t lowest = SIZE_MAX;
  std::queue<T> *chosen = &Queues[0]; // Default to first element
  for (auto qit = Queues.begin(); qit != Queues.end(); ++qit)
    if (qit->size() < lowest){
      lowest = qit->size();
      chosen = qit;
    }
  chosen->push(element);
}*/


namespace Par {  
  unsigned int GetCPUCount();

  template <class T> class Queue {
  public:
    Queue() : qcount(4), qsize(0), active(0) { init(); }
    Queue(const size_t queuecount) : 
      qcount(queuecount), qsize(0), active(0) { init(); }
    template <template <typename, typename = std::allocator<T>> class Container>
    Queue(const Container<T> &elements) : 
      qcount(4), qsize(elements.size()), active(0) {
      init();
      push_back(elements);
    }
    template <template <typename, typename = std::allocator<T>> class Container>
    Queue(const Container<T> &elements, const size_t queuecount) : 
      qcount(queuecount), qsize(elements.size()), active(0) {
      init();
      push_back(elements);
    }
    ~Queue() { delete[] queues; }
    
    void push_back(const T &element) {
      queues[active].push_back(element);
      ++active %= qcount;
      qsize++;
    }  

    template <template <typename, typename = std::allocator<T>> class Container>
    void push_back(const Container<T> &elements) {
      for (auto eit = elements.begin(); eit != elements.end(); ++eit)
	push_back(*eit);
    }

    inline size_t size() const { return qsize; }
    inline size_t size(size_t queue) const { return queues[queue].size(); }
    inline T& front(size_t queue){ return queues[queue].front(); }
    inline void pop(size_t queue){
      if(!queues[queue].size()) return;
      queues[queue].pop_front();
      qsize--;
    }
  
  private:
    const size_t qcount;
    size_t qsize;
    size_t active;
    std::deque<T>* queues;
    inline void init() { queues = new std::deque<T>[qcount];
      for(int i = 0; i < qcount; i++)
	queues[i] = std::deque<T>(0);
    }
  };
  
  typedef std::function<void(void)> Work;
  typedef Queue<Work> WorkQueue;
  template class Queue<std::function<void(void)>>;

  class Engine {
  public:
    Engine();
    Engine(size_t threads);
    ~Engine();
    void Run();
    void PushWork(Work work);
    
    // Flag for creation of a new Par::Queue
    inline void NewQueue() { queuestarted = false; }
  private:
    bool queuestarted;
    const size_t threadcount;
    std::deque<WorkQueue*> workqueues;
    void run(WorkQueue* queue, size_t index);
  };
} // namespace Par

#endif
