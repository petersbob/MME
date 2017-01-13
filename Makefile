CC=g++
CFLAGS=-g -c -Wall -std=gnu++11
LDFLAGS=-g -Wall -std=gnu++11

SOURCES=smashbot.cpp
GOALS=Goals/*.cpp
STRATS=Strategies/*.cpp
TACTICS=Tactics/*.cpp
CHAINS=Chains/*.cpp
UTIL=Util/*.cpp
OSC=osc/*.cpp
IP=ip/*.cpp

EXECUTABLE=smashbot

all: osc ip util main
	g++ -o smashbot osc/OscTypes.o osc/OscOutboundPacketStream.o ip/posix/UdpSocket.o ip/IpEndpointName.o ip/posix/NetworkingUtils.o *.o

.PHONY: main
.PHONY: util
.PHONY: osc
.PHONY: ip

main:
	$(CC) $(CFLAGS) $(SOURCES)

util:
	$(CC) $(CFLAGS) $(UTIL)

osc:
	g++ -Wall -Wextra -O3 -I. -DOSC_DETECT_ENDIANESS    -c -o osc/OscTypes.o osc/OscTypes.cpp
	g++ -Wall -Wextra -O3 -I. -DOSC_DETECT_ENDIANESS    -c -o osc/OscOutboundPacketStream.o osc/OscOutboundPacketStream.cpp
	g++ -Wall -Wextra -O3 -I. -DOSC_DETECT_ENDIANESS    -c -o ip/posix/UdpSocket.o ip/posix/UdpSocket.cpp
	g++ -Wall -Wextra -O3 -I. -DOSC_DETECT_ENDIANESS    -c -o ip/IpEndpointName.o ip/IpEndpointName.cpp
	g++ -Wall -Wextra -O3 -I. -DOSC_DETECT_ENDIANESS    -c -o ip/posix/NetworkingUtils.o ip/posix/NetworkingUtils.cpp



clean:
	rm -f *.o */*.o *.d */*.d smashbot
