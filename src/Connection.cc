#include <utility>

#include <websocket/Channel.h>
#include <websocket/Connection.h>
//#include <websocket/Message.h>

namespace websocket {

Connection::Connection(Channel && channel)
  : _channel(std::move(channel))
  {}

bool Connection::is_established() const {
  return _established;
}

bool Connection::establishing() const {
  return _incoming.size() >= 1;
}

std::size_t Connection::incomingSize() const {
  return _incoming.size();
}

Message & Connection::getLastIncomingMsg() {
  return _incoming.back();
}

void Connection::appendIncoming(Message && msg) {
  _incoming.push_back(msg);
}

} // websocket
