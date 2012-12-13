
#include "user_config.h"

int cdata;
int init_drone=1;
int init_counter=0;
int drone_status=2;
int old_time=0;
int seq  = 1;
char data[100];
int ledPin = 13;                  // LED test pin
 int rxPin = 19;                    // RX PIN 
 int txPin = 18;     
void setup()
{
  //delay(10000);
   pinMode(ledPin, OUTPUT);       // Initialize LED pin
   pinMode(rxPin, INPUT);
   pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(115200);
  //Serial.println("In Setup");
  //sprintf(data,"AT*LED=%d,2,1073741824,3\r",1);
 // delay(30000);
}


void loop()
{
  //Serial.println("In Loop");
 if (init_drone == 1)
    {
      //Serial.println("About to Inilialize drone");
        initialise_drone();
       //Serial.println("Initialise finished!");
        return;
        
        
        // Send emergency shutdown/reset command
         //   sprintf(data, "AT*REF=%d,290717952\r", 1);
            
    }
   
    /*switch(drone_status)
    {
       case 0://take off 
             sprintf(data,"AT*REF=%d,290718208\r",seq++);
             
           if (old_time == 0)
                old_time = millis();
            
            if (millis() - old_time > 2000)
            {
                // make sure the watch dog command is sent
                drone_status = 1;
                old_time = 0;
            }
         break;
         
      case 1: //land
            sprintf(data, "AT*REF=%d,290717696\r", seq++);
            
            if (old_time == 0)
                old_time = millis();
            
            if (millis() - old_time > 2000)
            {
                // make sure the watch dog command is sent
                drone_status = 2;
                old_time = 0;
                sprintf(data, "AT*REF=%d,0\r", seq++);
            }
         break; 
         case 2://LED test
           sprintf(data, "AT*LED=%d,2,1073741824,10\r", seq++);
           drone_status=3;
           default: break;
      
    }
    Serial1.println(data);
    Serial.println(data);
    delay(100000);*/
/*
  if (Serial1.available())
  {
    data= Serial1.read();
    Serial.write(data);
  }
 else{
 */
 /*
Serial.println(cdata);
Serial1.println(cdata);
delay(10000);
*/
 
}

void initialise_drone()
{
   // Serial1.println("#_Initialising_Drone_!");
  //Serial.println("#_Initialising_Drone_!");
    // It seems the first commands send get cut off 
    // sometimes so send some dummy data.
    sprintf(data, "dummy_data_to_init_connection");
    
    //Serial.println("In Initialize drone");
    
    switch (init_counter)
    {
        case 1:
            sprintf(data, "cd /data/video/ardudrone");
            //Serial.println("Case 1");
            break;
        case 2: 
            sprintf(data, "./atcomproxy_arm -v -d /dev/tty");
            init_counter = 9;
            //Serial.println("Case 2");
            break;
            
        case 3: // Set drone ready to fly
            sprintf(data, "AT*CONFIG=%d\"general:navdata_demo\",\"TRUE\"\r", seq++);
            //Serial.println("Case 3");
            break;
/*            
        case X: // TODO: DO WE NEED PAIRING??!?
            sprintf(data, "AT*CONFIG=%d,\"network:owner_mac\",\"%s\"\r", seq++, FAKE_MAC);
            break;
*/            
        case 4: // Set outdoor/indoor, outdoor = no shell, indoor = with shell
            sprintf(data, "AT*CONFIG=%d,\"control:outdoor\",\"%s\"\rAT*CONFIG=%d,\"control:flight_without_shell\",\"%s\"\r",
                seq++,
                FLY_OUTDOOR,
                seq++,
                FLY_OUTDOOR);
                //Serial.println("Case 4");
            break;
            
        case 5: // Set yaw rate
            sprintf(data, "AT*CONFIG=%d,\"control:control_yaw\",\"%d\"\r", seq++, YAW_RATE);
            //Serial.println("Case 5");
            break;
            
        case 6: // Set throttle rate
            sprintf(data, "AT*CONFIG=%d,\"control:control_vz_max\",\"%d\"\r", seq++, THROTTLE_RATE);
            //Serial.println("Case 6");
            break;
            
        case 7: // Set pitch, roll rate
            sprintf(data, "AT*CONFIG=%d,\"control:euler_angle_max\",\"0.%d\"\r", seq++, PITCH_ROLL_RATE);
            //Serial.println("Case 7");
            break;
            
        case 8: // Set max altitude
            sprintf(data, "AT*CONFIG=%d,\"control:altitude_max\",\"%d\"\r", seq++, ALTITUDE_MAX);
            //Serial.println("Case 8");
            break;

            
        case 9: // Drone is ready , play LEDs
            //sprintf(data, "AT*LED=%d,2,1073741824,0\r", seq++);
            sprintf(data, "AT*LED=1,2,1073741824,0\r");
            //Serial.println("Case 9");
            break;

            
        case 10: // Set drone ready for flying
            init_drone = 0;
            drone_status = 2;
            //Serial.println("Case 10");
            //delay(1000000);
            break;
    }

    init_counter++;
    Serial1.println(data);
    Serial.println(data);
    // Blink slowly >med__(long)< to give signal of init
    digitalWrite(LEDPIN, HIGH);
    delay(1000);
    digitalWrite(LEDPIN, LOW);
    delay(600);
}
