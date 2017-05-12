#ifndef WEBSOCKET_SERVER__CHANNEL
#define WEBSOCKET_SERVER__CHANNEL

#include <map>

#include <event2/event.h>
#include <event2/bufferevent.h>


namespace websocket {

class Channel {

public:
  
  Channel() = default;
  Channel(const Channel &) = delete;
  Channel & operator=(const Channel &) = delete;
  Channel(Channel && other);
  Channel & operator=(Channel && other);
  
  Channel(int sockfd);
  
  ~Channel();
  
  operator int();
  
  void send(const Message & msg);
  void receive();
  
private:
    int                  _sockfd = -1;
    struct bufferevent * _bev    = nullptr;
};

} // websocket

#endif
