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

namespace websocket {

void receive_from_channel(struct bufferevent * bev, void * arg) {
  std::map<int, Connection> & connections = *reinterpret_cast<std::map<int, Connection> *>(arg);
  
}

void error_from_channel(struct bufferevent * bev, short error, void * arg) {
  std::map<int, Connection> & connections = *reinterpret_cast<std::map<int, Connection> *>(arg);
  
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
