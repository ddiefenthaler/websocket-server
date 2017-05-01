#ifndef WEBSOCKET_SERVER__CHANNEL
#define WEBSOCKET_SERVER__CHANNEL

#include <map>

#include <event2/event.h>
#include <event2/bufferevent.h>


namespace websocket {

class Connection;

class Channel {

public:
  
  Channel();
  
  Channel(int sockfd, struct event_base & base, std::map<int, Connection> & connections);
  
  Channel(const Channel &) = delete;
  
  Channel(Channel && other);
  
  Channel & operator=(const Channel &) = delete;
  
  Channel & operator=(Channel && other);
  
  ~Channel();
  
  operator int();
  
  //todo send_to
  
private:
    int                  _sockfd;
    struct bufferevent * _bev;
};

} // websocket

#endif
