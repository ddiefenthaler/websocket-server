
CXXFLAGS = --std=c++11 -c -g -Wall -I ./include/

LDFLAGS = -lpthread -levent -levent_pthreads

HFILES = main.h Config.h network.h thread.h Connection.h Channel.h Message.h TaskQueue.h internal/lockhelper.h
CXXFILES = main.cc network.cc thread.cc Channel.cc Message.cc

.PHONY: all clean rebuild touch_rebuild

all: websocket-server

websocket-server: $(CXXFILES:%.cc=build/%.o)
	g++ -o websocket-server $+ $(LDFLAGS)

build/%.o: src/%.cc $(HFILES:%=include/websocket/%) build/trigger_rebuild
	g++ -o $@ $(CXXFLAGS) $<

clean:
	rm -rf build/*
	touch build/trigger_rebuild

rebuild: touch_rebuild all

touch_rebuild:
	touch build/trigger_rebuild

