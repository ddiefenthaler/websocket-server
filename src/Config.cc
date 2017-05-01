#include <websocket/Config.h>

namespace websocket {

class Channel {
  
  short getPort() {
    return _port;
  }
  void  setPort(short port) {
    _port = port;
  }
  
  private:
    short _port = 80;
};

} // websocket
