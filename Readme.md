# WebSocket Server

The WebSocket protocol allows browsers and other clients to communicate with
a server in a similar fashion like normal network sockets.
It is defined in [RFC 6455](https://tools.ietf.org/html/rfc6455).
It operates on layer 7 of the OSI model.
For example it describes how the packages safely pass through proxys.
On the other hand the protocol describes own mechanisms of fragmentation.

## Goals

The goal of this project is to create a WebSocket server that is controllable
from the command line or from server side scripts. The two main parts are
- the connection managment so that the user can easily
use it without exactly knowing how WebSockets work,
- and the scheduling and load balancing for proper
handling of multiple connections at once.

### Additional goals

- A WebSocket client from the command line in a similar fashion like `openssl s_client`.
- A way to process the payload sent from clients and answering it appropriate.

## Building from Source

For now this repository is just an outline without any actual code.
There is no way of building this project yet.

## Cause of the project

A C++ practicum offered at the Ludwig-Maximilans-Universität München (Munich).
Describing webpage: [http://www.mnm-team.org/teaching/Praktika/2016ws/cpppc/](http://www.mnm-team.org/teaching/Praktika/2016ws/cpppc/)
