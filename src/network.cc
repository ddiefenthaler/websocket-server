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

void onreceive(struct bufferevent * bev, void * arg) {
  
  Channel & chn = *reinterpret_cast<Channel *>(arg);
  
  chn.receive();
}

void onerror(struct bufferevent * bev, short error, void * arg) {

  if(error & BEV_EVENT_EOF) {
    Connection & con = connections.find(bufferevent_getfd(bev))->second;
    con.close();
  } else  {
    connections.erase(connections.find(bufferevent_getfd(bev)));
  }
}

void close_channel(struct bufferevent * bev, void * arg) {

  // todo  get_bufferlength != 0 => error
  connections.erase(connections.find(bufferevent_getfd(bev)));
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
