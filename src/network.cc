#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <map>

#include <websocket/main.h>
#include <websocket/network.h>
#include <websocket/Config.h>
#include <websocket/Channel.h>
#include <websocket/Connection.h>
#include <websocket/Message.h>

namespace websocket {

void receive_from_channel(struct bufferevent * bev, void * arg) {

  Connection & connection = connections.find(bufferevent_getfd(bev))->second;

  struct evbuffer * input = bufferevent_get_input(bev);
  if(connection.is_established() || connection.establishing()) {
    if(connection.getIncompleteMsg() == nullptr) {
      int len = evbuffer_get_length(input);
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
        msg.setFullLength(buffer_begin[1] & 0x7F);
        buffer_pos = 2;
      } else if((buffer_begin[1] & 0x7F) == 126) {
        msg.setFullLength(*reinterpret_cast<short *>(buffer_begin+2));
        buffer_pos = 4;
      } else if((buffer_begin[1] & 0x7F) == 127) {
        msg.setFullLength(*reinterpret_cast<long long *>(buffer_begin+2));
        buffer_pos = 10;
      }
      if(buffer_begin[1] & 0x80) {
        msg.setMask(buffer_begin+buffer_pos);
        buffer_pos += 4;
      }

      evbuffer_drain(input, buffer_pos);

      auto payload = msg.getPayload();
      len = evbuffer_get_length(input);
      if(len >= msg.getFullLength()) {
        payload.resize(msg.getFullLength());
        evbuffer_remove(input, payload.data(), msg.getFullLength());

        tq.push(0, Task(bufferevent_getfd(bev),std::move(msg)));
      } else {
        payload.resize(len);
        evbuffer_remove(input, payload.data(), len);

        connection.setIncompleteMsg(std::move(msg));
      }
    } else { // Complete last msg
      Message & msg = *connection.getIncompleteMsg();
      auto payload = msg.getPayload();
      int oldsize = payload.size();
      int remaining = msg.getFullLength() - oldsize;
      int len = evbuffer_get_length(input);
      if(len >= remaining) {
        payload.resize(oldsize + remaining);
        evbuffer_remove(input, payload.data() + oldsize, remaining);

        tq.push(0, Task(bufferevent_getfd(bev),std::move(msg)));
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
      bufferevent_setwatermark(bev, EV_READ, evbuffer_get_length(input)+1, 16384);
      return;
    } else {
      Message msg(OpeningHandshake_Client,p.pos+4);

      evbuffer_remove(input, msg.getPayload().data(),p.pos+4);

      bufferevent_setwatermark(bev, EV_READ, 0, 16384);

      tq.push(0,std::move(msg));
    }
  }

}

void error_from_channel(struct bufferevent * bev, short error, void * arg) {

  //if(error & BEV_EVENT_EOF) {
    connections.erase(connections.find(bufferevent_getfd(bev)));
  //} else  {
  //}
}

void accept_new_connection(evutil_socket_t sockfd, short event, void * arg) {
  struct sockaddr_storage saddr;
  socklen_t slen = sizeof(saddr);
  int fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&saddr), &slen);

  evutil_make_socket_nonblocking(fd);
  Channel ch(fd);
  connections.insert(std::make_pair(fd, Connection(std::move(ch))));
}

evutil_socket_t create_listen_socket() {
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = 0; // 0.0.0.0
  sin.sin_port = htons(config.getPort());

  evutil_socket_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
  evutil_make_socket_nonblocking(sockfd);

  int enable = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

  bind(sockfd, reinterpret_cast<const struct sockaddr*>(&sin), sizeof(sin));

  listen(sockfd, 16);

  return sockfd;
}

} //websocket
