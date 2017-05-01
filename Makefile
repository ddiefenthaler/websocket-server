
CXXFLAGS = -c -Wall -I ./include/

LDFLAGS = -lpthread -levent

CXXFILES = main.cc Config.cc network.cc Connection.cc Channel.cc

.PHONY: all

all: $(CXXFILES:%.cc=build/%.o)
	g++ -o websocket-server $+ $(LDFLAGS)

build/%.o: src/%.cc
	g++ -o $@ $(CXXFLAGS) $+
