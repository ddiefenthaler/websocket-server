#ifndef WEBSOCKET_SERVER__CONFIG
#define WEBSOCKET_SERVER__CONFIG

namespace websocket {

class Config {
  
  short getPort();
  void  setPort(short port);
  
  private:
    short _port = 80;
};

} // websocket

#endif
