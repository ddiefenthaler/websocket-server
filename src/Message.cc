#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <functional>
#include <algorithm>
#include <regex>

#include <boost/uuid/sha1.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <websocket/Message.h>
#include <websocket/main.h>
#include <websocket/Connection.h>

namespace websocket {

void Message::demask() {
  if(_masked) {
    int i = -1;
    auto mask = _mask;
    std::transform(_payload.begin(), _payload.end(),
                   _payload.begin(),
                   [&mask,&i](const unsigned char & p){
                     ++i;
                     if(i >= 4) {
                       i=0;
                     }
                     return p^mask[i];
                   });
    _masked = false;
  }
}

void Message::handle(int connection) {
  demask();

  auto & handler = Message::handlers.at(_type);
  // todo error handling
  handler(connection, *this);
}

void handle_OpeningHandshake_Client(int connection, Message & msg) {
  Connection & con = connections.find(connection)->second;
  auto & msg_payload = msg.getPayload();
  // regex parsing
  // from RFC 2616
  std::string LWS     = "(?:(?:\\r\\n)?[ \\t]+)";
  std::string TEXT    = "(?:[^\\x00-\\x1f\\x7f]|"+LWS+")";
  std::string token   = "(?:[^\\x00-\\x1f\\x7f()<>@,;:\\\\\"/[\\]?={} ]+)";
  std::string header  = "(?:("+token+"):"+LWS+"?("+TEXT+"*?)"+LWS+"?\\r\\n)";
  std::string request = "GET (.+) HTTP/1.1\\r\\n"+header+"*\\r\\n";
  // todo adjust to comply with RFC 2616
  std::regex request_re(request, std::regex::icase);
  std::match_results<decltype(msg_payload.begin())> m;
  // required header-fields todo seperate struct
  bool successful = true;
  bool got_host = false;
  bool got_upgrade = false;
  bool got_connection = false;
  bool got_sec_websocket_key = false;
  bool got_sec_websocket_version = false;
  std::string sec_websocket_accept;
  if(std::regex_match(msg_payload.begin(),msg_payload.end(),m,request_re)) {
    //std::cout << m.str(1) << '\n';
    // parse header fields
    std::regex header_re(header);
    std::regex_iterator<decltype(msg_payload.begin())> first_header_field(msg_payload.begin(),msg_payload.end(),header_re);
    std::regex_iterator<decltype(msg_payload.begin())> last_header_field;
    for(auto i = first_header_field; i != last_header_field; ++i) {
      auto & match = *i;
      std::string field_name = match.str(1);
      std::string field_value;
      std::regex lws_re(LWS);
      std::regex_replace(std::back_inserter(field_value),match[2].first,match[2].second,lws_re," ");
      //std::cout << field_name << ": " << field_value << std::endl;
      // parse seperate headers
      if(std::regex_match(field_name,std::regex("Host", std::regex::icase))) {
        got_host = true;
        // todo match host to config
      } else if(std::regex_match(field_name,std::regex("Upgrade", std::regex::icase))) {
        got_upgrade = true;
        if(!std::regex_search(field_value,std::regex("websocket", std::regex::icase))) {
          successful = false;
          break;
        }
      } else if(std::regex_match(field_name,std::regex("Connection", std::regex::icase))) {
        got_connection = true;
        if(!std::regex_search(field_value,std::regex("Upgrade", std::regex::icase))) {
          successful = false;
          break;
        }
      } else if(std::regex_match(field_name,std::regex("Sec-WebSocket-Key", std::regex::icase))) {
        got_sec_websocket_key = true;

        field_value += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

        boost::uuids::detail::sha1 sha1;
        sha1.process_bytes(field_value.data(),field_value.size());
        unsigned int hash_carr[5] = {0};
        sha1.get_digest(hash_carr);

        using namespace boost::archive::iterators;

        typedef base64_from_binary<
                  transform_width<
                    unsigned int *,
                    6,
                    32
                  >
                > b64enc_iter;

        std::string res;
        std::copy(b64enc_iter(&hash_carr), b64enc_iter(&hash_carr+1), back_inserter(res));
        res.append((4-(res.size()%4))%4,'=');

        sec_websocket_accept = res;
      } else if(std::regex_match(field_name,std::regex("Sec-WebSocket-Version", std::regex::icase))) {
        got_sec_websocket_version = true;
        if(!std::regex_match(field_value,std::regex("13", std::regex::icase))) {
          successful = false;
          break;
        }
      } else {
        // todo handle other key value pairs
        // ignore other header fields for now
      }
    }
  }

  if(successful && got_host && got_upgrade && got_connection &&
      got_sec_websocket_key && got_sec_websocket_version) {
    Message open_response(OpeningHandshake_Server);
    auto & or_payload = open_response.getPayload();
    std::string or_payload_str(
      "HTTP/1.1 101 Switching Protocols\r\n"
      "Server: wip-websocket-server\r\n"
      "Upgrade: websocket\r\n"
      "Connection: Upgrade\r\n"
      "Sec-WebSocket-Accept: "
    );
    or_payload_str.append(sec_websocket_accept);
    or_payload_str.append("\r\n\r\n");
    or_payload = std::vector<char>(or_payload_str.begin(),or_payload_str.end());

    con.establish(open_response);

    return;
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
  br_payload = std::vector<char>(br_payload_str.begin(),br_payload_str.end());
  con.send(bad_request);

  con.close();
}

void handle_OpeningHandshake_Server(int connection, Message & msg) {
  // todo client mode
}

#include "UserMessages.cc"

/*
 * todo dynamic user handling
 * for now compile time
void handle_Continuation(int connection, Message & msg) {
  // todo user handling
}

void handle_UserMessage_Text(int connection, Message & msg) {
  // todo user handling
}

void handle_UserMessage_Binary(int connection, Message & msg) {
  // todo user handling
}
*/

void handle_ControlMessage_Close(int connection, Message & msg) {
  Connection & con = connections.find(connection)->second;
  if(!con.is_closed()) {
    Message close_msg(ControlMessage_Close);
    con.send(close_msg);

    con.close();
  }
  // todo logging
  // todo masking for client mode
  // todo close only when having this msg received
  // (client mode only, RFC: The server MUST close the ... immediately)
}

void handle_ControlMessage_Ping(int connection, Message & msg) {
  Connection & con = connections.find(connection)->second;
  msg.setType(ControlMessage_Pong);
  // todo masking for client mode
  con.send(msg);
}

void handle_ControlMessage_Pong(int connection, Message & msg) {
  // todo check for previous ping / clear timeouts
}

std::unordered_map<MessageType,std::function<void(int,Message &)>>
create_handlers() {
  std::unordered_map<MessageType,std::function<void(int,Message &)>> res;

  res[OpeningHandshake_Client] = handle_OpeningHandshake_Client;
  res[OpeningHandshake_Server] = handle_OpeningHandshake_Server;
  res[Continuation] = handle_Continuation;
  res[UserMessage_Text] = handle_UserMessage_Text;
  res[UserMessage_Binary] = handle_UserMessage_Binary;
  res[ControlMessage_Close] = handle_ControlMessage_Close;
  res[ControlMessage_Ping] = handle_ControlMessage_Ping;
  res[ControlMessage_Pong] = handle_ControlMessage_Pong;

  return res;
}

std::unordered_map<MessageType,std::function<void(int,Message &)>>
Message::handlers = create_handlers();

}

