
#include <sstream>

#include <websocket/thread.h>
#include <websocket/Config.h>
#include <websocket/Connection.h>
#include <websocket/TaskQueue.h>

namespace websocket {

void worker_thread(const Config & config, TaskQueue & tq, std::map<int,Connection> & connections) {
  while(true) {
    handle(tq.take(), connections);
  }
}

void handle(Task t, std::map<int,Connection> & connections) {
  switch(t.type) {
    case IncomingMessage_User:
      handle_user(t.channel);
      break;
    case IncomingMessage_Control:
      handle_control(t.channel);
      break;
    case IncomingMessage_Open:
      handle_open(t.channel);
      break;
  }
}

handle_open(int channel, std::map<int,Connection> & connections) {
  Message & msg = connections.at(channel).takeFirstIncomingMsg();
  std::string http_request(msg.getPayload().data());
  std::istringstream ss(http_request);
  
  std::string currentField;
  
  while(std::getline(ss, currentField, '\n')) {
    // todo
  }
}

}
