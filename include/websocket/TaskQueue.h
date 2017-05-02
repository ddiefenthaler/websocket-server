#ifndef WEBSOCKET_SERVER__TASKQUEUE
#define WEBSOCKET_SERVER__TASKQUEUE

#include <array>
#include <deque>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include <websocket/Config.h>
#include <websocket/Message.h>

namespace websocket {

struct Task {
  int channel;
  Message msg;
  
  Task(int _channel, Message && _msg)
  : channel(_channel), msg(_msg)
  {}
};

template <std::size_t prios>
class TaskQueue {

public:

  TaskQueue() = default;
  
  void push(std::size_t prio, Task && task) {
    if(prio < prios) {
      std::lock_guard<std::mutex> lock(_mutex);
      _queues[prio].push_back(task);
      _cv.notify_all();
    }
  }
  
  Task take() {
    std::unique_lock<std::mutex> lock(_mutex);
    ++_waiting;
    _cv.wait(lock, [&]{
      for(int i=0; i < prios; i++) {
        if(_queues[i].size > 0) {
          return true;
        }
        // If only one thread is waiting only handle prio 0
        if(_waiting <= 1) {
          return false;
        }
      }
    });
    --waiting;
    
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
  std::atomic<unsigned short>        _waiting = std::atomic<unsigned short>(0);
  std::mutex                         _mutex;
  std::condition_variable            _cv;

};

} // websocket

#endif
