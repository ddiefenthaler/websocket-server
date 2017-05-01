#include <event2/event.h>
#include <event2/bufferevent.h>

#include <websocket/Channel.h>
#include <websocket/network.h>

namespace websocket {

class Channel {
  
  public:
  
  Channel()
  : _sockfd(-1), _bev(nullptr)
  {}
  
  Channel(int sockfd, struct event_base & base)
  : _sockfd(sockfd)
  {
    _bev = bufferevent_socket_new(&base, sockfd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, receive_from_channel, nullptr, error_from_channel, nullptr);
    bufferevent_setwatermark(bev, EV_READ, 0, 16384);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
  }
  
  Channel(const Channel &) = delete;
  
  Channel(Channel && other) {
    _sockfd = other.sockfd;
    _bev = other._bev;
    other._sockfd = -1;
    other._bev = nullptr;
  }
  
  Channel & operator=(const Channel &) = delete;
  
  Channel & operator=(Channel && other) {
    _sockfd = other.sockfd;
    _bev = other._bev;
    other._sockfd = -1;
    other._bev = nullptr;
    return *this;
  }
  
  ~Channel() {
    if(bev != nullptr) {
      bufferevent_free(bev);
    }
  }
  
  operator int() {
    return _sockfd;
  }
  
  private:
    int                  _sockfd;
    struct bufferevent * _bev;
};

} // websocket
