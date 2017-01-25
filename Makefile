CC=g++
CFLAGS=-g -c -Wall  -std=gnu++11
OSCFLAGS=-Wall -Wextra -O3 -I. -DOSC_DETECTENDIANESS -c 
LDFLAGS=-g -Wall -std=gnu++11

SOURCES=mme.cpp
UTIL=Util/*.cpp
OSC=Osc/OscTypes.cpp Osc/OscOutboundPacketStream.cpp
OSCOBJECTS=$(OSC:.cpp=.o)
IP=Ip/posix/UdpSocket.cpp Ip/IpEndpointName.cpp Ip/posix/NetworkingUtils.cpp
IPOBJECTS=$(IP:.cpp=.o)

EXECUTABLE=mme

all: osc ip util main
	g++ -o $(EXECUTABLE) *.o

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
	rm -f *.o */*.o *.d */*.d $(EXECUTABLE)
