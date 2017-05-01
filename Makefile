
CXXFLAGS = -Wall -I ./include/

LDFLAGS = 

CXXFILES = main.cc Config.cc network.cc Connection.cc Channel.cc

.PHONY: all

all: $(CXXFILES:%.cc=build/%.o)
	ld -o websocket-server $+ -levent

build/%.o: src/%.cc
	g++ -o $@ $(CFLAGS) $+
