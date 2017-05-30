#ifndef WEBSOCKET_SERVER__LIBEVENT
#define WEBSOCKET_SERVER__LIBEVENT

#include <event2/event.h>

namespace websocket {

class event_base_wrapper {

public:
  event_base_wrapper() {
    _base = event_base_new();
  }

  ~event_base_wrapper() {
    event_base_free(_base);
  }

  struct event_base & getBase() {
    return *_base;
  }

private:
  struct event_base * _base;

};

}

#endif
