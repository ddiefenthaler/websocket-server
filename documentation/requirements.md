# Websocket Server

- Managing connections according to [RFC 6455](https://tools.ietf.org/html/rfc6455)
    - Opening/Closing connections
    - Receiving/Sending messages
- Task based processing of messages
- ~~API for easy processing and responding to messages~~
    - Compile time function pointers instead
    - Perhaps shared objects to dynamically exchange behavior at startup-time/runtime