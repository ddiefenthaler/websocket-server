#ifndef WEBSOCKET_SERVER__CONFIG
#define WEBSOCKET_SERVER__CONFIG

namespace websocket {

class Config {

  public:
    short getPort() const;
    void  setPort(short port);
    
  private:
    short _port = 8080;
};

} // websocket

#endif
