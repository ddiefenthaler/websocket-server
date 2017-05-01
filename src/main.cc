#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <map>

#include <websocket/network.h>

/**
 * This file is for setting up the server,
 * loading the configuration and
 * starting the necessary threads.
 */

int main(int argc, char * argv[]) {
  //ProgramArgs programargs(argc, argv);
  Config      config; // default configuration
  //config.load(programargs.getConfigFile());
  //config.apply(programargs);
  
  std::map<int, Connection> connections;
  
  //start worker threads
  
  struct event_base * base;
  
  std::pair<std::map<int, Connection> *, struct event_base *> callback_args = make_pair(&connections, base);
  
  evutil_socket_t sockfd = websocket::create_listen_socket(config);
  
  struct event * listen_event = event_new(base, sockfd, EV_READ|EV_PERSIST, websocket::accept_new_connection, (void *)&callback_args);
  
  event_add(listen_event,nullptr);
  
  event_base_dispatch(base);
}