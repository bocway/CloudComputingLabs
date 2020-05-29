CXXFLAGS+=-O2 -ggdb -DDEBUG
CXXFLAGS+=-Wall -Wextra

all: kvstore2pcsystem

kvstore2pcsystem: main.cc participant.cc coordinator.cc protocolutil.cc -lpthread
	sudo g++ -O2 -o $@ $^ 

clean:
	sudo $(RM) kvstore2pcsystem