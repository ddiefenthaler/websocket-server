#include <vector>
#include <array>

#include <websocket/Message.h>

namespace websocket {

Message::Message(MessageType type, std::size_t size /*= 0*/)
  : _type(type), _payload(size)
  {}

std::vector<unsigned char> & Message::getPayload() {
  return _payload;
}

bool Message::isComplete() const {
  return _fully;
}

void Message::setType(MessageType type) {
  _type = type;
}
void Message::setFin(bool fin) {
  _fin = fin;
}
void Message::setMasked(bool masked) {
  _masked = masked;
}
void Message::setFullLength(long long length) {
  _full_length = length;
}

void Message::setMask(unsigned char * mask) {
  for(int i=0; i < 4; i++) {
    _mask[i] = mask[i];
  }
}

void Message::setComplete(bool fully) {
  _fully = fully;
}

long long Message::getFullLength() const {
  return _full_length;
}


}

