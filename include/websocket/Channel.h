#ifndef WEBSOCKET_SERVER__CHANNEL
#define WEBSOCKET_SERVER__CHANNEL

#include <event2/bufferevent.h>

#include <map>

#include <websocket/Message.h>


namespace websocket {

class Channel {

public:
  
  Channel() = delete;
  Channel(const Channel &) = delete;
  Channel & operator=(const Channel &) = delete;
  Channel(Channel && other);
  Channel & operator=(Channel && other);
  
  Channel(int sockfd);
  
  ~Channel();
  
  operator int();
  
  void send(const Message & msg);
  void receive();
  void close();
  
private:
    int                  _sockfd = -1;
    struct bufferevent * _bev    = nullptr;
};

} // websocket

#endif
