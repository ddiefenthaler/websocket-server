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

Message * Connection::getIncompleteMsg() {
  if(_incompleteIncoming) {
    return &_incompleteMsg;
  }
  return nullptr;
}

void Connection::setIncompleteMsg(Message && msg) {
  _incompleteIncoming = true;
  _incompleteMsg = msg;
}

void Connection::unsetIncompleteMsg() {
  _incompleteIncoming = false;
}

} // websocket
