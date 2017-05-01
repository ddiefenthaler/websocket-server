
#include <websocket/Channel.h>
//#include <websocket/Message.h>

namespace websocket {

class Connection {
  
  Connection() = delete;
  
  Connection(int sockfd);
  
  private:
    bool established = false;
    Channel _channel;
//    std::vector<Message> _incoming;
//    std::vector<Message> _outgoing;
};

} // websocket
