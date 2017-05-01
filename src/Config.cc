#include <websocket/Config.h>

namespace websocket {

short Config::getPort() const {
    return _port;
  }
void  Config::setPort(short port) {
    _port = port;
  }

} // websocket
