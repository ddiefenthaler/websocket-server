#ifndef WEBSOCKET_SERVER__CONNECTION
#define WEBSOCKET_SERVER__CONNECTION

#include <deque>

#include <websocket/Channel.h>
#include <websocket/Message.h>

namespace websocket {

class Connection {

public:

  Connection() = default;
  Connection(const Connection &) = delete;
  Connection & operator=(const Connection &) = delete;
  Connection(Connection && other) = default;
  Connection & operator=(Connection && other) = default;

  Connection(Channel && channel)
  : _channel(std::move(channel))
  {}

  inline bool is_established() const {
    return _established;
  }
  inline bool establishing() const {
    return _establishing;
  }
  inline bool is_closed() const {
    return _closed;
  }

  inline void set_established(bool ested) {
    _established = ested;
  }
  inline void set_establishing(bool esting) {
    _establishing = esting;
  }

  inline Message * getIncompleteMsg() {
    // pointer as Maybe-type
    if(_incompleteIncoming) {
      return &_incompleteMsg;
    }
    return nullptr;
  }
  inline void setIncompleteMsg(Message && msg) {
    _incompleteIncoming = true;
    _incompleteMsg = msg;
  }
  inline void unsetIncompleteMsg() {
    _incompleteIncoming = false;
  }

  inline void send(const Message & msg) {
    _channel.send(msg);
  }

  inline void close() {
    _closed = true;
    _channel.close();
  }

  inline operator int() {
    return _channel;
  }

private:
  bool _established  = false;
  bool _establishing = false;
  bool _closed       = false;
  Channel _channel;
  bool _incompleteIncoming = false;
  Message _incompleteMsg;
};

// ConIt is an iterator with value_type std::pair<int,Connection>
// ConSet ist a class wich has operator== overloaded
// If (*it == exclude) no message is sent to this Connection
template <class ConIt, class ConSet>
void broadcast(const Message & msg, ConIt first, ConIt last, const ConSet exclude) {
  for(ConIt it = first; it != last; ++it) {
    auto & con = (*it).second;
    if(!(exclude == con)) {
      con.send(msg);
    }
  }
}

} // websocket

#endif
