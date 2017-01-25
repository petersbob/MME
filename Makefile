CC=g++
CFLAGS=-g -c -Wall  -std=gnu++11
OSCFLAGS=-Wall -Wextra -O3 -I. -DOSC_DETECTENDIANESS -c 
LDFLAGS=-g -Wall -std=gnu++11

SOURCES=smashbot.cpp
GOALS=Goals/*.cpp
STRATS=Strategies/*.cpp
TACTICS=Tactics/*.cpp
CHAINS=Chains/*.cpp
UTIL=Util/*.cpp
OSC=osc/OscTypes.cpp osc/OscOutboundPacketStream.cpp
OSCOBJECTS=$(OSC:.cpp=.o)
IP=ip/posix/UdpSocket.cpp ip/IpEndpointName.cpp ip/posix/NetworkingUtils.cpp
IPOBJECTS=$(IP:.cpp=.o)

EXECUTABLE=smashbot

all: osc ip goals strats tactics chains main util
	$(CC) $(LDFLAGS) *.o -o $(EXECUTABLE)

.PHONY: main
.PHONY: util
.PHONY: osc
.PHONY: ip
.PHONY: goals
.PHONY: strats
.PHONY: tactics
.PHONY: chains
.PHONY: util
.PHONY: clean

main:
	$(CC) $(CFLAGS) $(SOURCES)

goals:
	$(CC) $(CFLAGS) $(GOALS)

strats:
	$(CC) $(CFLAGS) $(STRATS)

tactics:
	$(CC) $(CFLAGS) $(TACTICS)

chains:
	$(CC) $(CFLAGS) $(CHAINS)

util:
	$(CC) $(CFLAGS) $(UTIL)

osc:
	$(CC) $(OSCFLAGS) $(OSC)

ip:
	$(CC) $(OSCFLAGS) $(IP)

clean:
	rm -f *.o */*.o *.d */*.d smashbot
