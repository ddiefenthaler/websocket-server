
/**
 * simple chat
 */

void handle_Continuation(int connection, Message & msg) {
  broadcast(msg,connections.begin(),connections.end(),connection);
}

void handle_UserMessage_Text(int connection, Message & msg) {
  broadcast(msg,connections.begin(),connections.end(),connection);
}

void handle_UserMessage_Binary(int connection, Message & msg) {
  broadcast(msg,connections.begin(),connections.end(),connection);
}

