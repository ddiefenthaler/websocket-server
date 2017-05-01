#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <map>

#include <websocket/network.h>
#include <websocket/Config.h>
#include <websocket/Channel.h>
#include <websocket/Connection.h>
#include <websocket/Message.h>

namespace websocket {

void receive_from_channel(struct bufferevent * bev, void * arg) {
  std::map<int, Connection> & connections = *reinterpret_cast<std::map<int, Connection> *>(arg);

  Connection & connection = connections.find(bufferevent_getfd(bev))->second;

  struct evbuffer * input = bufferevent_get_input(bev);
  if(connection.is_established() || connection.establishing()) {

  } else {
    struct evbuffer_ptr p;
    evbuffer_ptr_set(input, &p, 0, EVBUFFER_PTR_SET);
    p = evbuffer_search(input, "\r\n\r\n", 4, &p);
    if(p.pos == -1) {
      bufferevent_setwatermark(bev, EV_READ, evbuffer_get_length(input)+1, 16384);
    } else {
      Message msg(OpeningHandshake_Client,p.pos+4);

      evbuffer_remove(input, msg.getPayloadPtr(),p.pos+4);

      // todo enque_incoming
      // todo enque task
    }
  }

}

void error_from_channel(struct bufferevent * bev, short error, void * arg) {
  std::map<int, Connection> & connections = *reinterpret_cast<std::map<int, Connection> *>(arg);
  
  //if(error & BEV_EVENT_EOF) {
    connections.erase(connections.find(bufferevent_getfd(bev)));
  //} else  {
  //}
}

void accept_new_connection(evutil_socket_t sockfd, short event, void * arg) {
  auto & args = *reinterpret_cast<std::pair<std::map<int, Connection> *, struct event_base *> *>(arg);
  std::map<int, Connection> & connections = *args.first;
  struct event_base & base = *args.second;
  struct sockaddr_storage saddr;
  socklen_t slen = sizeof(saddr);
  int fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&saddr), &slen);
  
  evutil_make_socket_nonblocking(fd);
  Channel ch(fd,base);
  connections[static_cast<int>(ch)] = Connection(std::move(ch));
}

evutil_socket_t create_listen_socket(const websocket::Config & config) {
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
