#ifndef WEBSOCKET_SERVER__MESSAGE
#define WEBSOCKET_SERVER__MESSAGE

#include <vector>

namespace websocket {

enum MessageType {
  OpeningHandshake_Client,
  OpeningHandshake_Server,
  UserMessage_Text,
  UserMessage_Binary,
  ControlMessage_Close,
  ControlMessage_Ping,
  ControlMessage_Pong
};

class Message {

public:

  Message() = default;

  Message(MessageType type, std::size_t size = 0);

  char * getPayloadPtr();

private:

  MessageType       _type;
  std::vector<char> _payload;
};

}

#endif
