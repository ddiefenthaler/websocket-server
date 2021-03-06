#ifndef WEBSOCKET_SERVER__INTERNAL__LOCKHELPER
#define WEBSOCKET_SERVER__INTERNAL__LOCKHELPER

#include <event2/bufferevent.h>

namespace websocket {

namespace internal {

class bufferevent_lockhelper {
public:
  bufferevent_lockhelper() = delete;
  bufferevent_lockhelper(const bufferevent_lockhelper & other) = delete;
  bufferevent_lockhelper & operator=(const bufferevent_lockhelper & other) = delete;
  bufferevent_lockhelper(bufferevent_lockhelper && other) {
    _bev = other._bev;
    other._bev = nullptr;
  }
  bufferevent_lockhelper & operator=(bufferevent_lockhelper && other) {
    _bev = other._bev;
    other._bev = nullptr;
    return *this;
  }

  bufferevent_lockhelper(struct bufferevent * bev)
  : _bev(bev)
  {}

  void lock() {
    bufferevent_lock(_bev);
  }

  void unlock() {
    if(_bev != nullptr) {
      bufferevent_unlock(_bev);
    }
  }

private:
  struct bufferevent * _bev;

};

} // internal

} // websocket

#endif
