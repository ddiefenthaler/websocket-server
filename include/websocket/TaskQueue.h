#ifndef WEBSOCKET_SERVER__TASKQUEUE
#define WEBSOCKET_SERVER__TASKQUEUE

#include <array>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>

struct Task {
  int channel;
  Message msg;
  
  Task(int _channel, Message && _msg)
  : type(_type), msg(_msg)
  {}
}

template <std::size_t prios>
class TaskQueue {

public:

  TaskQueue() = default;
  
  push(std::size_t prio, Task task) {
    if(prio < prios) {
      std::lock_guard<std::mutex> lock(_mutex);
      _queues[prio].push_back(Task);
      _cv.notify_all();
    }
  }
  
  Task take() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [&]{
      for(int i=0; i < prios; i++) {
        if(_queues[i].size > 0) {
          return true;
        }
      }
    });
    
    // todo atomic to get currently working threads
    
    for(int i=0; i < prios; i++) {
      if(_queues[i].size > 0) {
        Task & t = _queues[i].front();
        _queues[i].pop_front();
        return t;
      }
    }
  }

private:
  std::array<std::deque<Task>,prios> _queues;
  std::mutex                         _mutex;
  std::condition_variable            _cv;
  

};

#endif
