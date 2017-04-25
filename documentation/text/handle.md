## Package handling

Currently there are six different frame types defined in the WebSocket protocol.
- Two different user message types,
- Three different control frames and
- One that specifies that the frame is just a continuation of the previous one.

The first technique that comes to mind is pattern matching.
This would be easily possible in C++ using types and overloaded functions.
Unfortunatly the messages that are processed cannot be categorized
during the compile time. In the runtime when this categorization happens
the program has no idea about types. This means that this approach
is not that easy to realise.

It would be nice if this code snippet would work.
Exactly the above described problem arises.
```c++
void handle(GenericMessage & m) {
  handle(reinterpret_cast<typename MessageType<m.opcode>::type>(m));
}
```

Instead the code would have to look like this.
The code repeats itself very often.
Luckily it is only relevant for five or six differnt values in this case.
```c++
void handle(GenericMessage & m) {
  if(m.opcode == WS_CLOSE) {
    handle(reinterpret_cast<MessageType<WS_CLOSE>::type>(m));
  } else if(m.opcode == WS_PING) {
    handle(reinterpret_cast<MessageType<WS_PING>::type>(m));
  } else if(m.opcode == WS_PONG) {
    handle(reinterpret_cast<MessageType<WS_PONG>::type>(m));
  } else if(m.opcode == WS_TEXT) {
    handle(reinterpret_cast<MessageType<WS_TEXT>::type>(m));
  } else if(m.opcode == WS_BIN) {
    handle(reinterpret_cast<MessageType<WS_BIN>::type>(m));
  }
}
```

There are more ways to get to a result like this.
But they all require some kind of duplicated code like this.
This is a little bit startling as the complier can easily deal with such a case
if the parameters are known at the compiletime.

### Configfuration of package reaction

During the opening handshake of the WebSocket protocol the client and the server
agree on a subprotocol to use. This protocol is layered on top of WebSocket.
This can be used to determine which function to call while handling the messages.

These function can be loaded dynamically at runtime from shared objects.
This makes the server very flexible as it does not require relaunching or recompiling.
There is no limit to the functions from the shared object.
This means it can do calculations, store values and use files.
It should only avoid to block waiting on other messages from clients as this
may lead to a deadlock. In case of a deadlock the remaining parts of
the server keep running through priorities during scheduling.

In these shared objects there also may be functions to respond to messages
from the observer thread. This enables to also influence the server from
server side scripts. For example this way a client can only use long polling
and present the user a nearly identic experience to a native WebSocket client.

As an outlook instead of shared objects python scripts may be an alternative.
Such scripts are easier to create and are therefore also very interesting
for a flexible server.

