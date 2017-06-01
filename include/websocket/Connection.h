#ifndef WEBSOCKET_SERVER__CONNECTION
#define WEBSOCKET_SERVER__CONNECTION

#include <deque>

#include <websocket/Channel.h>
#include <websocket/Message.h>

namespace websocket {

class Connection {

public:

  Connection() = delete;
  Connection(const Connection &) = delete;
  Connection & operator=(const Connection &) = delete;
  Connection(Connection && other) = default;
  Connection & operator=(Connection && other) = default;

  Connection(Channel && channel)
  : _channel(std::move(channel))
  {}

  // todo split into seperate types for different states
  inline bool is_established() const {
    return _established;
  }
  inline bool establishing() const {
    return _establishing;
  }
  inline bool is_closed() const {
    return _closed;
  }

  inline void establish(Message & header) {
    send(header);
    _established = true;
  }
  inline void header_received() {
    _establishing = true;
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
