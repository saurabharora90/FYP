#include <Arduino.h>
#include <Streaming.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>

#define REMOTEHOST_PORT       3000     //Will send data to this port
#define LOCAL_PORT            2000     //Listens on port 2000
#define ADHOC_NAME            "Drone_Server"
#define ADHOC_CHANNEL         10

WiFly server;

void configureServer()
{
  SoftwareSerial wifiSerial (11,10);

  Serial.begin(9600);
  wifiSerial.begin(9600);
  server.begin(&wifiSerial);
  //Set port on which to listen for data
  server.setPort(LOCAL_PORT);
  server.setIpProtocol(WIFLY_PROTOCOL_UDP);
  
  //Create Ad hoc does the following:
  //setDHCP(WIFLY_DHCP_MODE_OFF);
  //setIP(F("169.254.1.1"));
  //setNetmask(F("255.255.0.0"));

  server.createAdhocNetwork(ADHOC_NAME, ADHOC_CHANNEL);
  
  Serial.println(F("Ad Hoc set" ) );
  
}

WiFly getServerPointer()
{
  return server;
}
