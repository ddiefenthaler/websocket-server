#include <vector>
#include <array>

#include <websocket/Message.h>
#include <websocket/internal/circular_iterator.h>

namespace websocket {

void Message::demask() {
  if(_masked) {
    std::transfrom(_payload.begin(), _payload.end(),
                   internal::circular_iterator(_mask.begin(),_mask.end()),
                   [](unsigned char p, unsigned char m){
                     return p^m;
                   })
  }
}

void Message::handle(int connection, int defered /* = 0 */) {
  if(defered != 0) {
    // invalid recursion
    return;
  }

  demask();

  if(_type == OpeningHandshake_Client) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<OpeningHandshake_Client>::type *>(this);
    typeMsg.handle(connection,1);
  } else if(_type == OpeningHandshake_Server) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<OpeningHandshake_Server>::type *>(this);
    typeMsg.handle(connection,1);
  } else if(_type == Continuation) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<Continuation>::type *>(this);
    typeMsg.handle(connection,1);
  } else if(_type == UserMessage_Text) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<UserMessage_Text>::type *>(this);
    typeMsg.handle(connection,1);
  } else if(_type == UserMessage_Binary) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<UserMessage_Binary>::type *>(this);
    typeMsg.handle(connection,1);
  } else if(_type == ControlMessage_Close) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<ControlMessage_Close>::type *>(this);
    typeMsg.handle(connection,1);
  } else if(_type == ControlMessage_Ping) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<ControlMessage_Ping>::type *>(this);
    typeMsg.handle(connection,1);
  } else if(_type == ControlMessage_Pong) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<ControlMessage_Pong>::type *>(this);
    typeMsg.handle(connection,1);
  } else {
    // todo error handling
  }
}

void OpenHandshakeClientMsg::handle(int connection, int defered) {
  // regex parsing
}

void OpenHandshakeServerMsg::handle(int connection, int defered) {
  // todo client mode
}

void ContinuationMsg::handle(int connection, int defered) {
  // todo user handling
}

void TextUserMsg::handle(int connection, int defered) {
  // todo user handling
}

void BinaryUserMsg::handle(int connection, int defered) {
  // todo user handling
}

void CloseControlMsg::handle(int connection, int defered) {
  // close connection
}

void PingControlMsg::handle(int connection, int defered) {
  // respond with pong
}

void PongControlMsg::handle(int connection, int defered) {
  // todo check for previous ping / clear timeouts
}

}

