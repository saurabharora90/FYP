/*This program connects to a AdHoc connection created by a WiFly Board whose IP address we know.
The current WiFly board makes use of a static IP. This way multiple boards can be connected to the same IP and can be programmed to send data to
other boards as each is making use of a static IP and we know those IP address.

Other possible method is to have a static IP for the adHoc creator and the others to send their IP address and MAC-address to the server when the connect to it and 
the server storing them to know which one to send the information too.*/

#include <Arduino.h>
#include <Streaming.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>

#define REMOTEHOST_PORT       2000
#define LOCAL_PORT            2000
#define ADHOC_NAME            "newTest1"
#define ADHOC_CHANNEL         10

SoftwareSerial wifiSerial (11,10);
WiFly wifly;

void setup()
{
  char buf[1024];
  
  Serial.begin(9600);
  wifiSerial.begin(9600);
  wifly.begin(&wifiSerial);
  
  Serial.print("Device Id: ");
  Serial.println(wifly.getDeviceID(buf, sizeof(buf)));
	
/* Setup the WiFly to connect to the adHoc wifi network */
	
  Serial.println("Leaving network");
  wifly.leave();
  
  //Connect to Adhoc
  wifly.setSSID(ADHOC_NAME); //Ad Hoc created by the other WiFly board
  wifly.setAuthMode(0); //0 is Open. The ad hoc network is open
  wifly.setIP(F("169.254.100.1")); //Set a static IP.
  wifly.setNetmask(F("255.255.0.0"));
  wifly.setDHCP(WIFLY_DHCP_MODE_OFF);
  wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);
  wifly.save();
  
  Serial.println("Parameters set");
  Serial.println("Joining network....");
  
  wifly.join();
  Serial.println(F("Ad Hoc joined" ) );
  Serial.println(wifly.getSSID(buf, sizeof(buf)));
  Serial.println(wifly.getIP(buf, sizeof(buf)));
  
  Serial.println("Sending data");
  wifly.sendto("Hello, world", "169.254.1.1", REMOTEHOST_PORT); //The IP address of the WiFly board which created the ad-hoc connection.
  //wifly.sendto("Hello, world", "169.254.182.24", REMOTEHOST_PORT); Test on laptop
  Serial.println("Data sent");
}

void loop()
{  
  //wifly.sendto("Hello, world", "192.168.1.1", REMOTEHOST_PORT);
}
