#include <Arduino.h>
#include <Streaming.h>
#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
#include "system_config.h"

//WiFly GX Breakout Board
WiFly wifly;

void setup()
{
  //Set up the ad-hoc connections for UDP Communication.
  WiFi_setup();
  
  //set up compass
  
  //set up GPS
  
  //set up the drone
  init_system();
}

void loop()
{
}

void WiFi_setup()
{
  if(Drone_Type=="Server")
  {
     configureServer();
     wifly = getServerPointer();
  }
  else if(Drone_Type=="Client")
  {
    configureClient();
    wifly = getClientPointer();
  }
}
