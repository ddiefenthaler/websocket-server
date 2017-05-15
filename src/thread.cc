
#include <websocket/thread.h>
#include <websocket/main.h>
#include <websocket/TaskQueue.h>
#include <websocket/Message.h>

namespace websocket {

void worker_thread() {
  while(true) {
    Task t = tq.take();
    t.msg.handle(t.channel);
  }
}

}
