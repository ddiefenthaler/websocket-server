#include <utility>

#include <websocket/Connection.h>
#include <websocket/Channel.h>
//#include <websocket/Message.h>

namespace websocket {

Connection::Connection(Channel && channel)
  : _channel(std::move(channel))
  {}

} // websocket
