## Introduction

A large portion of the modern web pages are dynamic.
To accomplish this JavaScript is used to update the page.
Sometimes whole pages display completely dynamic content.
Very often this updates to this content depend on external factors.
This updates have to get loaded from the server again.

To receive this update one way would be to simply reload the page.
A more hidden way is to reload the content in an invisible iframe or
just use an asynchron HTTP request. But this has a large overhead
as each request has easily twice the size of the actual update.
This is espacially the case if the new content should be displayed as fast
as possible and it is unpredictable for the client when to search for it.
For example this is the case in a chat application.
To minimise the overhead the request blocks and
the server only sends a response if new information are available.
This approach is called long polling.

WebSockets are a way to get rid of most of this overhead.
By just opening one connection and keeping it open only one request is needed.
Through this two way communication is possible triggered by any of server or client.
The WebSocket protocol is specified in [RFC 6455](https://tools.ietf.org/html/rfc6455).
In the OSI model it operates on the layers 5 through 7 as an application.
This classification is a litte bit odd as the protocol provides it own fragmentation mechanics.
For further information see [1.5. Design Philosophy](https://tools.ietf.org/html/rfc6455#section-1.5) in the referenced RFC.

The clients of a WebSocket connection are usually browsers.
As such web pages should be available in all larger browsers
the support for WebSockets is wide and embedded directly in the browser.
The interface how to use WebSockets from JavaScript is defined by the [w3c](https://www.w3.org/TR/websockets/).

On the other hand the wide spreaded HTTP servers do not provide
an implementation of WebSockets. To a certain extent that is reasonable
as two way communication is not the primary use case of a HTTP server.
The seperation of HTTP and WebSockets is easy as WebSockets usually use
a different port and the opening handshake looks like a HTTP request.
A server that does not support WebSockets can correctly answer such a request
with an apporpriate HTTP status code.

The goal of this practical course is to create an explicit WebSocket server.
This allows specific optimazation for this use case.
Nevertheless it is kept in mind that this project should be easily usabl
along with classic HTTP servers like apache and nginx.
Therefore this program should provide an command line interface
which allows easy and full control from server side scripts.

