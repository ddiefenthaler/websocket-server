
BOOST_SUBLIBS = $(wildcard boost/*/)
BOOST_HEADER  = boost/uuid/include/boost/uuid/sha1.hpp

CXXFLAGS = --std=c++11 -c -g -Wall -I ./include/ $(BOOST_SUBLIBS:%=-I %include/)

LDFLAGS = -lpthread -levent -levent_pthreads

HFILES = main.h libevent.h Config.h network.h thread.h Connection.h Channel.h Message.h TaskQueue.h internal/lockhelper.h
CXXFILES = main.cc network.cc thread.cc Channel.cc Message.cc

.PHONY: all clean rebuild touch_rebuild

all: websocket-server

websocket-server: $(CXXFILES:%.cc=build/%.o)
	g++ -o websocket-server $+ $(LDFLAGS)

build/%.o: src/%.cc $(HFILES:%=include/websocket/%) $(BOOST_HEADER) build/trigger_rebuild
	g++ -o $@ $(CXXFLAGS) $<

clean:
	rm -rf build/*
	touch build/trigger_rebuild

rebuild: touch_rebuild all

touch_rebuild:
	touch build/trigger_rebuild

$(BOOST_HEADER): # pull_boost
	git submodule init
	git submodule update

