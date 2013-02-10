/*This program connects to a AdHoc connection created by a WiFly Board whose IP address we know.
The current WiFly board makes use of a static IP. This way multiple boards can be connected to the same IP and can be programmed to send data to
other boards as each is making use of a static IP and we know those IP address.

Other possible method is to have a static IP for the adHoc creator and the others to send their IP address and MAC-address to the server when the connect to it and 
the server storing them to know which one to send the information too.*/

#include <Arduino.h>
#include <Streaming.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>

#include "system_config.h"

WiFly client;

void configureClient()
{
  SoftwareSerial wifiSerial (11,10);
  
  char buf[1024];
  
  Serial.begin(9600);
  wifiSerial.begin(9600);
  client.begin(&wifiSerial);
  
  Serial.print("Device Id: ");
  Serial.println(client.getDeviceID(buf, sizeof(buf)));
	
/* Setup the WiFly to connect to the adHoc wifi network */
	
  Serial.println("Leaving network");
  client.leave();
  
  //Connect to Adhoc
  client.setSSID(ADHOC_NAME); //Ad Hoc created by the other WiFly board
  client.setAuthMode(0); //0 is Open. The ad hoc network is open
  client.setIP(F("169.254.100.1")); //Set a static IP.
  client.setNetmask(F("255.255.0.0"));
  client.setDHCP(WIFLY_DHCP_MODE_OFF);
  client.setPort(Client_PORT);  //Configure it to also be able to listen to UDP dat
  client.setIpProtocol(WIFLY_PROTOCOL_UDP);
  client.save();

  Serial.println("Parameters set");
  Serial.println("Joining network....");
  
  client.join();
  Serial.println(F("Ad Hoc joined" ) );
  Serial.println(client.getSSID(buf, sizeof(buf)));
  Serial.println(client.getIP(buf, sizeof(buf)));

  Serial.println("Sending data");
  client.sendto("Connecting...", "169.254.1.1", Server_PORT); //The IP address of the WiFly board which created the ad-hoc connection.
  //wifly.sendto("Hello, world", "169.254.182.24", REMOTEHOST_PORT); Test on laptop
  Serial.println("Data sent");
}

WiFly getClientPointer()
{
  return client;
}
