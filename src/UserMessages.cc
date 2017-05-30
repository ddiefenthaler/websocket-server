
/**
 * simple chat
 */

void ContinuationMsg::handle(int connection, int defered) {
  broadcast(*this,connections.begin(),connections.end(),connection);
}

void TextUserMsg::handle(int connection, int defered) {
  broadcast(*this,connections.begin(),connections.end(),connection);
}

void BinaryUserMsg::handle(int connection, int defered) {
  broadcast(*this,connections.begin(),connections.end(),connection);
}

