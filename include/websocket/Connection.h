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

private:
  bool _established = false;
  Channel _channel;
  std::deque<Message> _incoming;
};

} // websocket

#endif
