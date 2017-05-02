#ifndef WEBSOCKET_SERVER__CONNECTION
#define WEBSOCKET_SERVER__CONNECTION

#include <deque>

#include <websocket/Channel.h>
#include <websocket/Message.h>

namespace websocket {

class Connection {

public:

  Connection() = default;

  Connection(Channel && channel);

  bool is_established() const;

  bool establishing() const;

  Message * getIncompleteMsg();

  void setIncompleteMsg(Message && msg);

  void unsetIncompleteMsg();

private:
  bool _established = false;
  bool _establishing = false;
  Channel _channel;
  bool _incompleteIncoming = false;
  Message _incompleteMsg;
};

} // websocket

#endif
