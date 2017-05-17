#include <vector>
#include <array>
#include <algorithm>
#include <regex>

#include <websocket/Message.h>
#include <websocket/main.h>
#include <websocket/Connection.h>
#include <websocket/internal/circular_iterator.h>

namespace websocket {

void Message::demask() {
  if(_masked) {
    std::transform(_payload.begin(), _payload.end(),
                   make_circular_iterator(_mask.begin(),_mask.end()),
                   _payload.begin(),
                   [](const unsigned char & p, const unsigned char & m){
                     return p^m;
                   });
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
    typedMsg.handle(connection,1);
  } else if(_type == OpeningHandshake_Server) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<OpeningHandshake_Server>::type *>(this);
    typedMsg.handle(connection,1);
  } else if(_type == Continuation) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<Continuation>::type *>(this);
    typedMsg.handle(connection,1);
  } else if(_type == UserMessage_Text) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<UserMessage_Text>::type *>(this);
    typedMsg.handle(connection,1);
  } else if(_type == UserMessage_Binary) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<UserMessage_Binary>::type *>(this);
    typedMsg.handle(connection,1);
  } else if(_type == ControlMessage_Close) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<ControlMessage_Close>::type *>(this);
    typedMsg.handle(connection,1);
  } else if(_type == ControlMessage_Ping) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<ControlMessage_Ping>::type *>(this);
    typedMsg.handle(connection,1);
  } else if(_type == ControlMessage_Pong) {
    auto & typedMsg = *reinterpret_cast<CppMessageType<ControlMessage_Pong>::type *>(this);
    typedMsg.handle(connection,1);
  } else {
    // todo error handling
  }
}

void OpenHandshakeClientMsg::handle(int connection, int defered) {
  Connection & con = connections.find(connection)->second;
  auto & msg_payload = getPayload();
  // regex parsing
  // todo adjust to comply with RFC 2616
  std::basic_regex<unsigned char> get_request_re(reinterpret_cast<const unsigned char *>("GET .* HTTP/1\\.1\\r\\n([^[:space:]]+[[:space:]]*:[[:space:]]*.+\\r\\n)+\\r\\n"));
  if(std::regex_match(msg_payload.begin(),msg_payload.end(),get_request_re)) {
    // parse header fields
    std::basic_regex<unsigned char> header_field(reinterpret_cast<const unsigned char *>("([^[:space:]]+)[[:space:]]*:[[:space:]]*(.+)\\r\\n"));
    std::regex_iterator<decltype(msg_payload.begin())> first_header_field(msg_payload.begin(),msg_payload.end(),header_field);
    std::regex_iterator<decltype(msg_payload.begin())> last_header_field;
    for(auto i = first_header_field; i != last_header_field; ++i) {
      // parse seperate headers
    }
  }

  Message bad_request(OpeningHandshake_Server);
  auto & br_payload = bad_request.getPayload();
  // todo outsource in seperate Header
  std::string br_payload_str(
    "HTTP/1.1 400 Bad Request\r\n"
    "Server: wip-websocket-server\r\n"
    "Content-Length: 52\r\n"
    "\r\n"
    "Invalid opening handshake for a websocket connection"
  );
  br_payload = std::vector<unsigned char>(br_payload_str.begin(),br_payload_str.end());
  con.send(bad_request);
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

