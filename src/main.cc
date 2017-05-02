#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <map>
#include <utility>

#include <main.h>
#include <websocket/network.h>

/**
 * This file is for setting up the server,
 * loading the configuration and
 * starting the necessary threads.
 */

int main(int argc, char * argv[]) {
  //ProgramArgs programargs(argc, argv);
  //websocket::Config      config; // default configuration
  //websocket::config.load(programargs.getConfigFile());
  //websocket::config.apply(programargs);
  
  //std::map<int, websocket::Connection> connections;
  
  //start worker threads
  
  evthread_use_pthreads();
  
  websocket::base = event_base_new();
  
  evutil_socket_t sockfd = websocket::create_listen_socket();
  
  struct event * listen_event = event_new(websocket::base, sockfd, EV_READ|EV_PERSIST, websocket::accept_new_connection, nullptr);
  
  event_add(listen_event,nullptr);
  
  event_base_dispatch(websocket::base);
}
