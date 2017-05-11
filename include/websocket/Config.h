#ifndef WEBSOCKET_SERVER__CONFIG
#define WEBSOCKET_SERVER__CONFIG

namespace websocket {

class Config {

  public:
    inline short getPort() const{
      return _port;
    }
    inline void  setPort(short port) {
      _port = port;
    }
    
    inline unsigned short getNumberThreads() const {
      return _number_threads;
    }
    inline void  setNumberThreads()(unsigned short number_threads) {
      _number_threads = number_threads;
    }
    
  private:
    short          _port = 8080;
    unsigned short _number_threads = 4;
};

} // websocket

#endif
