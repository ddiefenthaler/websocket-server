#include <netinet/in.h>
#include <sys/socket.h>

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <map>
#include <vector>
#include <utility>

#include <websocket/main.h>
#include <websocket/libevent.h>
#include <websocket/network.h>
#include <websocket/thread.h>

/**
 * This file is for setting up the server,
 * loading the configuration and
 * starting the necessary threads.
 */

namespace websocket {

Config                    config;
std::map<int, Connection> connections;
TaskQueue<2>              tq;
struct event_base       * base;

}

int main(int argc, char * argv[]) {
  //ProgramArgs programargs(argc, argv);
  //websocket::Config      config; // default configuration
  //websocket::config.load(programargs.getConfigFile());
  //websocket::config.apply(programargs);
  
  //std::map<int, websocket::Connection> connections;
  
  std::vector<std::thread> worker_threads;
  for(int i=0; i < websocket::config.numberThreads(); i++) {
    worker_threads.push_back(std::thread(websocket::worker_thread));
  }
  
  evthread_use_pthreads();
  
  websocket::event_base_wrapper base_wrapper;
  websocket::base = &(base_wrapper.base());
  
  evutil_socket_t sockfd = websocket::create_listen_socket();
  
  struct event * listen_event = event_new(websocket::base, sockfd, EV_READ|EV_PERSIST, websocket::accept_new_connection, nullptr);
  
  event_add(listen_event,nullptr);
  
  event_base_dispatch(websocket::base);
  
  for(auto & wt : worker_threads) {
      wt.join();
  }
}
