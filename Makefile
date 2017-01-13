CC=g++
CFLAGS=-g -c -Wall  -std=gnu++11
OSCFLAGS=-Wall -Wextra -O3 -I. -DOSC_DETECTENDIANESS -c 
LDFLAGS=-g -Wall -std=gnu++11

SOURCES=smashbot.cpp
UTIL=Util/*.cpp
OSC=osc/OscTypes.cpp osc/OscOutboundPacketStream.cpp
OSCOBJECTS=$(OSC:.cpp=.o)
IP=ip/posix/UdpSocket.cpp ip/IpEndpointName.cpp ip/posix/NetworkingUtils.cpp
IPOBJECTS=$(IP:.cpp=.o)

EXECUTABLE=smashbot

all: osc ip util main
	g++ -o smashbot *.o

.PHONY: main
.PHONY: util
.PHONY: osc
.PHONY: ip

main:
	$(CC) $(CFLAGS) $(SOURCES)

util:
	$(CC) $(CFLAGS) $(UTIL)

osc:
	$(CC) $(OSCFLAGS) $(OSC)

ip:
	$(CC) $(OSCFLAGS) $(IP)

clean:
	rm -f *.o */*.o *.d */*.d smashbot
