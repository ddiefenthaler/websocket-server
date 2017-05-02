#ifndef WEBSOCKET_SERVER__MAIN
#define WEBSOCKET_SERVER__MAIN

#include <websocket/Connection.h>
#include <websocket/TaskQueue.h>
#include <websocket/Config.h>

/** Holds global variables */

namespace websocket {

extern Config                    config;
extern std::map<int, Connection> connections;
extern TaskQueue<2>              tq;
extern struct event_base       * base;

}

#endif
