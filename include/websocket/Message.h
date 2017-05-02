#ifndef WEBSOCKET_SERVER__MESSAGE
#define WEBSOCKET_SERVER__MESSAGE

#include <vector>
#include <array>

namespace websocket {

enum MessageType : int {
  OpeningHandshake_Client = -1,
  OpeningHandshake_Server = -2,
  Continuation = 0x0,
  UserMessage_Text = 0x1,
  UserMessage_Binary = 0x2,
  ControlMessage_Close = 0x8,
  ControlMessage_Ping = 0x9,
  ControlMessage_Pong = 0xA
};

class Message {

public:

  Message() = default;

  Message(MessageType type, std::size_t size = 0);

  std::vector<unsigned char> & getPayload();

  void setType(MessageType type);
  void setFin(bool fin);
  void setMasked(bool masked);
  void setFullLength(long long length);
  void setMask(unsigned char * mask);

  long long getFullLength() const;

private:

  bool               _fin = true;
  MessageType        _type;
  bool               _masked = false;
  long long          _full_length;
  std::array<unsigned char,4> _mask {};
  std::vector<unsigned char>  _payload;
};

}

#endif
