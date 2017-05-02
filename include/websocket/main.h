#ifndef WEBSOCKET_SERVER__MAIN
#define WEBSOCKET_SERVER__MAIN

#include <websocket/Connection.h>
#include <websocket/TaskQueue.h>
#include <websocket/Config.h>

/** Holds global variables */

namespace websocket {

Config                    config;
std::map<int, Connection> connections;
TaskQueue<2>              tq;
struct event_base       * base;

}

#endif
