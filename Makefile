
CXXFLAGS = --std=c++11 -c -g -Wall -I ./include/

LDFLAGS = -lpthread -levent -levent_pthreads

HFILES = Config.h network.h thread.h Connection.h Channel.h Message.h thread.h
CXXFILES = main.cc network.cc thread.cc Channel.cc Message.cc

.PHONY: all clean rebuild

all: websocket-server

websocket-server: $(CXXFILES:%.cc=build/%.o)
	g++ -o websocket-server $+ $(LDFLAGS)

build/%.o: src/%.cc $(HFILES:%=include/websocket/%) build/trigger_rebuild
	g++ -o $@ $(CXXFLAGS) $<

clean:
	rm -rf build/*
	touch build/trigger_rebuild

rebuild:
	touch build/trigger_rebuild

