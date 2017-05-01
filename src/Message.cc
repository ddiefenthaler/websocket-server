#include <vector>

#include <websocket/Message.h>

namespace websocket {

Message::Message(MessageType type, std::size_t size /*= 0*/)
  : _type(type), _payload(size)
  {}

char * Message::getPayloadPtr() {
  return _payload.data();
}

}

