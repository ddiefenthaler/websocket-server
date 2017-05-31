#include <event2/bufferevent.h>

#include <limits>
#include <array>
#include <vector>
#include <mutex>

#include <boost/endian/conversion.hpp>

#include <websocket/main.h>
#include <websocket/Channel.h>
#include <websocket/Connection.h>
#include <websocket/Message.h>
#include <websocket/network.h>
#include <websocket/internal/lockhelper.h>

namespace websocket {

Channel::Channel(int sockfd)
: _sockfd(sockfd)
{
  _bev = bufferevent_socket_new(base, sockfd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
  bufferevent_setcb(_bev, onreceive, nullptr, onerror, this);
  bufferevent_setwatermark(_bev, EV_READ, 0, 16384);
  bufferevent_enable(_bev, EV_READ|EV_WRITE);
}

Channel::Channel(Channel && other) {
  _sockfd = other._sockfd;
  _bev = other._bev;
  internal::bufferevent_lockhelper lockhelper(_bev);
  std::lock_guard<internal::bufferevent_lockhelper> lock(lockhelper);
  bufferevent_setcb(_bev, onreceive, nullptr, onerror, this);
  other._sockfd = -1;
  other._bev = nullptr;
}

Channel & Channel::operator=(Channel && other) {
  _sockfd = other._sockfd;
  _bev = other._bev;
  internal::bufferevent_lockhelper lockhelper(_bev);
  std::lock_guard<internal::bufferevent_lockhelper> lock(lockhelper);
  bufferevent_setcb(_bev, onreceive, nullptr, onerror, this);
  other._sockfd = -1;
  other._bev = nullptr;
  return *this;
}

Channel::~Channel() {
  if(_bev != nullptr) {
    bufferevent_free(_bev);
  }
}

Channel::operator int() {
  return _sockfd;
}

void Channel::send(const Message & msg) {
  internal::bufferevent_lockhelper lockhelper(_bev);
  std::lock_guard<internal::bufferevent_lockhelper> lock(lockhelper);

  struct evbuffer * output = bufferevent_get_output(_bev);

  auto & payload = msg.getPayload();

  switch(msg.getType()) {
    case OpeningHandshake_Client:
    case OpeningHandshake_Server:
    {
      evbuffer_add(output, payload.data(), payload.size());
      break;
    }
    case Continuation:
    case UserMessage_Text:
    case UserMessage_Binary:
    case ControlMessage_Close:
    case ControlMessage_Ping:
    case ControlMessage_Pong:
    {
      unsigned char opcode = static_cast<unsigned char>(msg.getType());
      if(msg.isFin()) {
        opcode = opcode | 0x80;
      }
      unsigned char len;
      uint16_t len16 = 0;
      uint64_t len64 = 0;
      if(payload.size() < 126) {
        len = payload.size();
      } else if(payload.size() <= std::numeric_limits<uint16_t>::max()) {
        len = 126;
        len16 = payload.size();
        boost::endian::native_to_big_inplace(len16);
      } else if(payload.size() <= std::numeric_limits<uint64_t>::max()) {
        len = 127;
        len64 = payload.size();
        boost::endian::native_to_big_inplace(len64);
      } else {
        //todo error handling / fragmentation
      }
      if(msg.isMasked()) {
        len = len | 0x80;
      }

      evbuffer_add(output, &opcode, 1);
      evbuffer_add(output, &len, 1);
      if(len16 != 0) {
        evbuffer_add(output, &len16, 2);
      } else if(len64 != 0) {
        evbuffer_add(output, &len64, 8);
      }
      if(msg.isMasked()) {
        evbuffer_add(output, msg.getMask().data(), 4);
      }
      evbuffer_add(output, payload.data(), payload.size());
      break;
    }
    default:
      // todo error handling
      break;
  }
}

void Channel::receive() {
  Connection & connection = connections.find(_sockfd)->second;

  // todo state machine instead for better code readability
  // todo states from rfc: connecting, ...
  struct evbuffer * input = bufferevent_get_input(_bev);
  if(connection.is_established() || connection.establishing()) {
    if(connection.getIncompleteMsg() == nullptr) {
      unsigned long long len = evbuffer_get_length(input);
      unsigned char * buffer_begin;
      if(len < 14) {
        buffer_begin = evbuffer_pullup(input, -1);
      } else {
        buffer_begin = evbuffer_pullup(input, 14);
      }

      // todo better length check
      Message msg;
      msg.setFin(buffer_begin[0] & 0x80);
      msg.setType(static_cast<MessageType>(buffer_begin[0] & 0x0F));
      msg.setMasked(buffer_begin[1] & 0x80);
      int buffer_pos;
      if((buffer_begin[1] & 0x7F) < 126) {
        msg.setChunkLength(buffer_begin[1] & 0x7F);
        buffer_pos = 2;
      } else if((buffer_begin[1] & 0x7F) == 126) {
        auto chunkLength = boost::endian::big_to_native(*reinterpret_cast<uint16_t *>(buffer_begin+2));
        msg.setChunkLength(chunkLength);
        buffer_pos = 4;
      } else if((buffer_begin[1] & 0x7F) == 127) {
         auto chunkLength = boost::endian::big_to_native(*reinterpret_cast<uint64_t *>(buffer_begin+2));
        msg.setChunkLength(chunkLength);
        buffer_pos = 10;
      }
      if(buffer_begin[1] & 0x80) {
        msg.setMask(buffer_begin+buffer_pos);
        buffer_pos += 4;
      }

      evbuffer_drain(input, buffer_pos);

      auto & payload = msg.getPayload();
      len = evbuffer_get_length(input);
      if(len >= msg.getChunkLength()) {
        payload.resize(msg.getChunkLength());
        evbuffer_remove(input, payload.data(), msg.getChunkLength());

        tq.push(0, Task(_sockfd,std::move(msg)));
      } else {
        payload.resize(len);
        evbuffer_remove(input, payload.data(), len);

        connection.setIncompleteMsg(std::move(msg));
      }
    } else { // Complete last msg
      Message & msg = *connection.getIncompleteMsg();
      auto payload = msg.getPayload();
      int oldsize = payload.size();
      int remaining = msg.getChunkLength() - oldsize;
      int len = evbuffer_get_length(input);
      if(len >= remaining) {
        payload.resize(oldsize + remaining);
        evbuffer_remove(input, payload.data() + oldsize, remaining);

        tq.push(0, Task(_sockfd,std::move(msg)));
        connection.unsetIncompleteMsg();
      } else {
        payload.resize(oldsize + len);
        evbuffer_remove(input, payload.data() + oldsize, len);
      }
    }
  } else { // openening connection
    struct evbuffer_ptr p;
    evbuffer_ptr_set(input, &p, 0, EVBUFFER_PTR_SET);
    p = evbuffer_search(input, "\r\n\r\n", 4, &p);
    if(p.pos == -1) {
      bufferevent_setwatermark(_bev, EV_READ, evbuffer_get_length(input)+1, 16384);
      return;
    } else {
      Message msg(OpeningHandshake_Client,p.pos+4);

      evbuffer_remove(input, msg.getPayload().data(),p.pos+4);

      bufferevent_setwatermark(_bev, EV_READ, 0, 16384);

      tq.push(0,Task(_sockfd,std::move(msg)));

      connection.set_establishing(true);
    }
  }

}

void Channel::close() {
  bufferevent_setwatermark(_bev, EV_WRITE, 1, 16384);
  bufferevent_setcb(_bev, onreceive, close_channel, onerror, this);
}

} // websocket
