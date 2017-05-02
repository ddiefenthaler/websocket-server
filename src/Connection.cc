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
  return _establishing;
}

std::size_t Connection::incomingSize() const {
  return _incoming.size();
}

Message * Connection::getIncompleteMsg() {
  if(_incompleteIncoming) {
    return &_msg;
  }
}

void Connection::setIncompleteMsg(Message && msg) {
  _incompleteIncoming = true;
  _msg = msg;
}

void Connection::unsetIncompleteMsg() {
  _incompleteIncoming = false;
}

} // websocket
