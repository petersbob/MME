#ifndef EXAMPLEPACKETLISTENER_H
#define EXAMPLEPACKETLISTENER_H

#define ADDRESS "127.0.0.1"
#define PORT 7000

#include "../Osc/OscReceivedElements.h"
#include "../Osc/OscPacketListener.h"

class ExamplePacketListener : public osc::OscPacketListener
{
 protected:

  virtual void ProcessMessage( const osc::ReceivedMessage& m,
			       const IpEndpointName& remoteEndpoint );
};

#endif
