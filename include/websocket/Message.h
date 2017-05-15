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
  typedef Message self_t;

  Message() = default;
  Message(const self_t &) = default;
  Message & operator=(const self_t &) = default;
  Message(self_t &&) = default;
  Message & operator=(self_t &&) = default;

  Message(MessageType type, std::size_t size = 0)
  : _type(type), _payload(size)
  {}

  inline const std::vector<unsigned char> & getPayload() const {
    return _payload;
  }

  inline std::vector<unsigned char> & getPayload() {
    return _payload;
  }

  inline void setType(MessageType type) {
    _type = type;
  }
  inline void setFin(bool fin) {
    _fin = fin;
  }
  inline void setMasked(bool masked) {
    _masked = masked;
  }
  inline void setFullLength(unsigned long long length) {
    _full_length = length;
  }
  inline void setMask(unsigned char * mask) {
    for(int i=0; i < 4; i++) {
      _mask[i] = mask[i];
    }
  }

  inline MessageType getType() const {
    return _type;
  }
  inline bool isFin() const {
    return _fin;
  }
  inline bool isMasked() const {
    return _masked;
  }
  inline unsigned long long getFullLength() const {
    return _full_length;
  }
  inline const std::array<unsigned char,4> & getMask() const {
    return _mask;
  }

  void demask();
  void handle(int connection, int defered = 0);

private:

  bool               _fin = true;
  MessageType        _type;
  bool               _masked = false;
  unsigned long long _full_length;
  std::array<unsigned char,4> _mask {};
  std::vector<unsigned char>  _payload;
};

class OpenHandshakeClientMsg : public Message {public: void handle(int connection, int defered = 1);};
class OpenHandshakeServerMsg : public Message {public: void handle(int connection, int defered = 1);};
class ContinuationMsg : public Message {public: void handle(int connection, int defered = 1);};
class TextUserMsg : public Message {public: void handle(int connection, int defered = 1);};
class BinaryUserMsg : public Message {public: void handle(int connection, int defered = 1);};
class CloseControlMsg : public Message {public: void handle(int connection, int defered = 1);};
class PingControlMsg : public Message {public: void handle(int connection, int defered = 1);};
class PongControlMsg : public Message {public: void handle(int connection, int defered = 1);};

template <MessageType t>
struct CppMessageType {
  using type = Message;
};

template <>
struct CppMessageType<OpeningHandshake_Client> {
  using type = OpenHandshakeClientMsg;
};

template <>
struct CppMessageType<OpeningHandshake_Server> {
  using type = OpenHandshakeServerMsg;
};

template <>
struct CppMessageType<Continuation> {
  using type = ContinuationMsg;
};

template <>
struct CppMessageType<UserMessage_Text> {
  using type = TextUserMsg;
};

template <>
struct CppMessageType<UserMessage_Binary> {
  using type = BinaryUserMsg;
};

template <>
struct CppMessageType<ControlMessage_Close> {
  using type = CloseControlMsg;
};

template <>
struct CppMessageType<ControlMessage_Ping> {
  using type = PingControlMsg;
};

template <>
struct CppMessageType<ControlMessage_Pong> {
  using type = PongControlMsg;
};

}

#endif
