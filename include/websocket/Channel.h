#include <event2/event.h>
#include <event2/bufferevent.h>

namespace websocket {

class Channel {
  
  Channel();
  
  Channel(int sockfd, struct event_base & base);
  
  Channel(const Channel &) = delete;
  
  Channel(Channel && other);
  
  Channel & operator=(const Channel &) = delete;
  
  Channel & operator=(Channel && other);
  
  ~Channel();
  
  operator int();
  
  //todo send_to
  
  private:
    int                  _sockfd;
    struct * bufferevent _bev;
};

} // websocket
