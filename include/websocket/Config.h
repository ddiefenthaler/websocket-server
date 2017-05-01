
namespace websocket {

class Channel {
  
  short getPort();
  void  setPort(short port);
  
  private:
    short _port = 80;
};

} // websocket
