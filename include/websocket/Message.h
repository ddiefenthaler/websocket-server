#ifndef WEBSOCKET_SERVER__MESSAGE
#define WEBSOCKET_SERVER__MESSAGE

#include <vector>
#include <array>
#include <unordered_map>
#include <functional>

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
  static std::unordered_map<MessageType,std::function<void(int,Message &)>> handlers;

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

  inline const std::vector<char> & payload() const {
    return _payload;
  }

  inline std::vector<char> & payload() {
    return _payload;
  }

  inline MessageType type(MessageType type) {
    _type = type;
    return _type;
  }
  inline bool fin(bool fin) {
    _fin = fin;
    return _fin;
  }
  inline bool masked(bool masked) {
    _masked = masked;
    return _masked;
  }
  inline unsigned long long chunkLength(unsigned long long length) {
    _chunk_length = length;
    return _chunk_length;
  }
  inline const std::array<unsigned char,4> & mask(unsigned char * mask) {
    for(int i=0; i < 4; i++) {
      _mask[i] = mask[i];
    }
    return _mask;
  }

  inline MessageType type() const {
    return _type;
  }
  inline bool isFin() const {
    return _fin;
  }
  inline bool isMasked() const {
    return _masked;
  }
  inline unsigned long long chunkLength() const {
    return _chunk_length;
  }
  inline const std::array<unsigned char,4> & mask() const {
    return _mask;
  }

  void demask();
  void handle(int connection);

private:

  bool               _fin = true;
  MessageType        _type;
  bool               _masked = false;
  unsigned long long _chunk_length;
  std::array<unsigned char,4> _mask {};
  std::vector<char>  _payload;
};


}

#endif
