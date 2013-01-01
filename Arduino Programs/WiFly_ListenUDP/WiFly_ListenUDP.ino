#include <Arduino.h>
#include <Streaming.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>

#define REMOTEHOST_PORT       3000
#define LOCAL_PORT            2000
#define ADHOC_NAME            "newTest1"
#define ADHOC_CHANNEL         10


SoftwareSerial wifiSerial (11,10);
WiFly wifly;

void setup()
{
  Serial.begin(9600);
  wifiSerial.begin(9600);
  wifly.begin(&wifiSerial);
  
  //Set port on which to listen for data
  wifly.setPort(LOCAL_PORT);
  wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);
  wifly.createAdhocNetwork(ADHOC_NAME, ADHOC_CHANNEL);
  
  Serial.println(F("Ad Hoc set" ) );
  
}

void loop()
{
  if (wifly.available() > 0) 
  {
    Serial.write(wifly.read());
  }
}

