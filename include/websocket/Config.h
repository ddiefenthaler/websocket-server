#ifndef WEBSOCKET_SERVER__CONFIG
#define WEBSOCKET_SERVER__CONFIG

namespace websocket {

class Config {

  public:
    inline short port() const{
      return _port;
    }
    inline short port(short port) {
      _port = port;
      return _port;
    }
    
    inline unsigned short numberThreads() const {
      return _number_threads;
    }
    inline unsigned short numberThreads(unsigned short number_threads) {
      _number_threads = number_threads;
      return _number_threads;
    }
    
  private:
    short          _port = 8080;
    unsigned short _number_threads = 4;
};

} // websocket

#endif
