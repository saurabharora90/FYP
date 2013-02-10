#include <Arduino.h>
#include <Streaming.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>

#include "system_config.h"

WiFly server;

void configureServer()
{
  SoftwareSerial wifiSerial (11,10);

  Serial.begin(9600);
  wifiSerial.begin(9600);
  server.begin(&wifiSerial);
  //Set port on which to listen for data
  server.setPort(Server_PORT);
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
