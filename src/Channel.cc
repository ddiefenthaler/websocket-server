#include <event2/event.h>
#include <event2/bufferevent.h>

#include <websocket/Channel.h>
#include <websocket/network.h>

namespace websocket {

Channel::Channel()
  : _sockfd(-1), _bev(nullptr)
  {}

Channel::Channel(int sockfd, struct event_base & base)
  : _sockfd(sockfd)
  {
    _bev = bufferevent_socket_new(&base, sockfd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(_bev, receive_from_channel, nullptr, error_from_channel, nullptr);
    bufferevent_setwatermark(_bev, EV_READ, 0, 16384);
    bufferevent_enable(_bev, EV_READ|EV_WRITE);
  }

Channel::Channel(Channel && other) {
    _sockfd = other._sockfd;
    _bev = other._bev;
    other._sockfd = -1;
    other._bev = nullptr;
  }

Channel & Channel::operator=(Channel && other) {
    _sockfd = other._sockfd;
    _bev = other._bev;
    other._sockfd = -1;
    other._bev = nullptr;
    return *this;
  }

Channel::~Channel() {
    if(_bev != nullptr) {
      bufferevent_free(_bev);
    }
  }

Channel::operator int() {
    return _sockfd;
  }

} // websocket
