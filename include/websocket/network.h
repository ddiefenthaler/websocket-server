#ifndef WEBSOCKET_SERVER__NETWORK
#define WEBSOCKET_SERVER__NETWORK

#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <websocket/network.h>
#include <websocket/Config.h>
#include <websocket/Channel.h>
#include <websocket/Connection.h>

namespace websocket {

void receive_from_channel(struct bufferevent * bev, void * arg);

void error_from_channel(struct bufferevent * bev, void * arg);

void accept_new_connection(evutil_socket_t sockfd, short event, void * arg);

evutil_socket_t create_listen_socket(const Config & config);

} //websocket

#endif
