#ifndef WEBSOCKET_SERVER__CONNECTION
#define WEBSOCKET_SERVER__CONNECTION

#include <websocket/Channel.h>
//#include <websocket/Message.h>

namespace websocket {

class Connection {

  public:

    Connection() = default;
    
    Connection(Channel && channel);
    
  private:
    bool established = false;
    Channel _channel;
//    std::vector<Message> _incoming;
//    std::vector<Message> _outgoing;
};

} // websocket

#endif
