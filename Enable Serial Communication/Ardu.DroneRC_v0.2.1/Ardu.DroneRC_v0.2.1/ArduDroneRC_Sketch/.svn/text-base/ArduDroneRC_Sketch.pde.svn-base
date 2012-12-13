/*
 *  Ardu.Drone RC V0.2.1
 *
 *  Created by nosaari on 16.02.11.
 *  (Based on RC Ar.Drone V2.1 by Mheeli at rcgroups.com)
 *
 *  Arduino sketch to control AR.Drone with your RC controller
 *  by connecting it to the drone with an arduino board.
 *
 */

#include "user_config.h"


enum ARDStatus
{
    ARDStatusInvalid,
    ARDStatusGrounded,              // On the ground
    ARDStatusTakingOff,             
    ARDStatusHovering,
    ARDStatusFlying,
    ARDStatusLanding,
    ARDStatusFlatTrim,
    ARDStatusResetWatchdog,         // Reset watchdog
    ARDStatusHoverResetWatchdog,    // Reset watchdog during long hover
    ARDStatusEmergencyReset,        // User Emergency state
    ARDStatusCount
};


int drone_status        = 0;
int init_counter        = 0;
int old_time            = 0;

int throttle            = 0;
int throttle_old        = 0;
int throttle_mid        = 0;
int throttle_step       = 0;
float throttle_move     = 0;

int roll                = 0;
int roll_old            = 0;
int roll_mid            = 0;
int roll_step           = 0;
float roll_move         = 0;

int pitch               = 0;
int pitch_old           = 0;
int pitch_mid           = 0;
int pitch_step          = 0;
float pitch_move        = 0;

int yaw                 = 0;
int yaw_old             = 0;
int yaw_mid             = 0;
int yaw_step            = 0;
float yaw_move          = 0;

int aux_1               = 0;
int aux_1_old           = 0;
int aux_2               = 0;
int aux_2_old           = 0;


int seq                 = 1;
int init_drone          = 1;
int flying_switch       = 0; // 0 = grounded, 1 = flying
int reset_switch        = 0;
int reset_fired         = 0;
int was_flying          = 0;
char data[100];
int stick_diff          = 0;
int radio_read_seq      = 0;


void setup()
{
    // Setup serial connection.
    // Set the speed of our serial link.
    Serial.begin(115200);
    
    // Define pins as input
    pinMode(THROTTLEPIN, INPUT);
    pinMode(ROLLPIN, INPUT);
    pinMode(PITCHPIN, INPUT);
    pinMode(YAWPIN, INPUT);
    pinMode(AUX1PIN, INPUT);
    pinMode(AUX2PIN, INPUT);
    
#ifdef DEBUG_LED
    // Set a pin for LED output
    pinMode(LEDPIN, OUTPUT);
#endif

    // For safety read radio values and only continue when sticks are in mid
    // position and take-off signal is false.
    while (1)
    {
        read_radio();
        
#ifdef DEBUG_LOG_SERIAL
        Serial.print("#_Throttle_");
        Serial.print(throttle);
        Serial.print("_Yaw_");
        Serial.print(yaw);
        Serial.print("_Roll_");
        Serial.print(roll);
        Serial.print("_Pitch_");
        Serial.print(pitch);
        Serial.print("_Aux-1_");
        Serial.print(aux_1);
        Serial.print("_Aux-2_");
        Serial.print(aux_2);
#endif
        
        throttle_mid    = throttle;
        yaw_mid         = yaw;
        roll_mid        = roll;
        pitch_mid       = pitch;
        
        // only continue if all sticks are in the middle and take off switch is deactivated
        if (aux_1 > 1500)
            stick_diff = 1;
            
        // TODO: Implement possibility to switch between indoor and outdoor config depending on state of aux_2!
        
        if (throttle > 1500)
        {
            if ((throttle - 1500) > 50)
                stick_diff = 1;
        }
        if (yaw > 1500)
        {
            if ((yaw - 1500) > 50)
                stick_diff = 1;
        }
        if (roll > 1500)
        {
            if ((roll - 1500) > 50)
                stick_diff = 1;
        }
        if (pitch > 1500)
        {
            if ((pitch - 1500) > 50)
                stick_diff = 1;
        }
        if (throttle < 1500)
        {
            if ((1500 - throttle) > 50)
                stick_diff = 1;
        }
        if (yaw < 1500)
        {
            if ((1500 - yaw) > 50)
                stick_diff = 1;
        }
        if (roll < 1500)
        {
            if ((1500 - roll) > 50)
                stick_diff = 1;
        }
        if (pitch < 1500)
        {
            if ((1500 - pitch) > 50)
                stick_diff = 1;
        }
        
        if (stick_diff == 0)
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("___All.Good!___");
#endif
            break;
        }
        
        stick_diff = 0;
        
#ifdef DEBUG_LOG_SERIAL
        Serial.println("___Please.move.sticks.to.neutral.position!___");
#endif

#ifdef DEBUG_LED
        // Blink led >short_short_pause< as long as we are not ready
        digitalWrite(LEDPIN, HIGH);
        delay(20);
        digitalWrite(LEDPIN, LOW);
        delay(40);
        digitalWrite(LEDPIN, HIGH);
        delay(20);
        digitalWrite(LEDPIN, LOW);
#endif
        delay(500);
    }
    
    // Steps for each channel
    yaw_step        = RC_YAW_MAX - RC_DEADZONE_YAW - yaw_mid;
    throttle_step   = RC_THROTTLE_MAX - RC_DEADZONE_THROTTLE - throttle_mid;
    roll_step       = RC_ROLL_MAX - RC_DEADZONE_ROLL - roll_mid;
    pitch_step      = RC_PITCH_MAX - RC_DEADZONE_PITCH - pitch_mid;
    
#ifdef DEBUG_LOG_SERIAL
        Serial.println("#_Setup.Complete_!");
#endif
    
#ifdef DEBUG_LED
    // Signalize end of setup with >short__short__long___(pause)___<
    digitalWrite(LEDPIN, HIGH);
    delay(10);
    digitalWrite(LEDPIN, LOW);
    delay(100);
    digitalWrite(LEDPIN, HIGH);
    delay(10);
    digitalWrite(LEDPIN, LOW);
    delay(100);
    digitalWrite(LEDPIN, HIGH);
    delay(200);
    digitalWrite(LEDPIN, LOW);
    delay(800);
#endif
}

void loop()
{
    // Wait until drone is initialized, init_drone will then be set to 0!
    if (init_drone == 1)
    {
        initialise_drone();
        
        return;
    }
    
    // Read radio values
    read_radio();
    
    if (flying_switch == 0)
    {
        drone_status = ARDStatusGrounded;
        if (was_flying == 1)
            was_flying = 0;       
    }

    if (reset_switch == 1 && reset_fired == 0)
    {
        // Remember that we flew so the flight switch must be set to 
        // land prior being able to start again.
        if (drone_status != ARDStatusGrounded)
            was_flying = 1;
        drone_status = ARDStatusEmergencyReset;
        reset_fired = 1;
    }
    else if (reset_switch == 0 && reset_fired == 1)
    {
        reset_fired = 0;
    }  
    
#ifdef DEBUG_LOG_SERIAL
    Serial.print("#_Yaw_");
    Serial.print(yaw);
    Serial.print("_Yaw.Move_");
    Serial.print(yaw_move);
    
    Serial.print("_Throttle_");
    Serial.print(throttle);
    Serial.print("_Throttle.Move_");
    Serial.print(throttle_move);
    
    Serial.print("_Roll_");
    Serial.print(roll);
    Serial.print("_Roll.Move_");
    Serial.print(roll_move);
    
    Serial.print("_Pitch_");
    Serial.print(pitch);
    Serial.print("_Pitch.Move_");
    Serial.print(pitch_move);
    
    Serial.print("_Aux-1_");
    Serial.print(aux_1);
    
    Serial.print("_Aux-2_");
    Serial.print(aux_2);
    
    Serial.print("___Drone.Status_");
    Serial.print(drone_status);
#endif
    
    switch (drone_status)
    {
        case ARDStatusEmergencyReset:
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("__!EMERGENCY.RESET!");
#endif

            // Send emergency shutdown/reset command
            sprintf(data, "AT*REF=%d,290717952\r", 1);

            // Set drone to grounded after sending emergency reset
            drone_status = ARDStatusGrounded;
                        
            break;
        }
      
      
        case ARDStatusGrounded:
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("__Grounded");
#endif
            // Keep sending keep alive and landing command
            sprintf(data, "AT*PCMD=%d,0,0,0,0,0\rAT*REF=%d,290717696\r", 1, 1);
            
            // If flying switch goes high then take off
            // was_flying is a security check that prevents drone
            // from launching again after emergency reset was pressed
            // while drone was in the air! The flight switch must be
            // set to land prio being able to start again!
            if (flying_switch == 1 && was_flying == 0)
            {
                // Reset watchdog, flat trim, take off and hover
                drone_status = ARDStatusResetWatchdog;
                old_time = 0;
            }
            
            break;
        }

            
        case ARDStatusResetWatchdog:
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("__Resetting.Watchdog");
#endif

            sprintf(data, "AT*COMWDG=%d\r", 1);
            
            // TODO: STILL REQUIRED? give WiFi some time to send the command
            if (old_time == 0)
                old_time = millis();
            
            if (millis() - old_time > 200)
            {
                // make sure the watch dog command is sent
                drone_status = ARDStatusFlatTrim;
                seq = 1;
                old_time = 0;
            }
            
            break;
        }
            
            
        case ARDStatusFlatTrim:
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("__Flat.Triming");
#endif
            sprintf(data, "AT*FTRIM=%d\rAT*LED=%d,2,1073741824,1\r", seq++, seq++);
            
            // TODO: STILL REQUIRED? give WiFi some time to send the flat trim command
            if (old_time == 0)
                old_time = millis();
            
            if (millis() - old_time > 200)
            {
                drone_status = ARDStatusTakingOff;
                old_time = 0;
            }
            
            break;
        }
            
            
        case ARDStatusTakingOff:
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("__Taking.Off");
#endif
            sprintf(data, "AT*PCMD=%d,0,%ld,%ld,%ld,%ld\rAT*REF=%d,290718208\r",
              seq++,
              roll_move,
              pitch_move,
              throttle_move,
              yaw_move,
              seq++);
            
            // TODO: STILL REQUIRED? give WiFi some time to send the flat trim command
            if (old_time == 0)
                old_time = millis();
            
            if (millis() - old_time > 150)
            {
                drone_status = ARDStatusHovering;
                old_time = 0;
            }
            
            break;
        }
            
            
        case ARDStatusHovering:
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("__Hovering");
#endif
            sprintf(data, "AT*PCMD=%d,0,%ld,%ld,%ld,%ld\r", seq++, roll_move, pitch_move, throttle_move, yaw_move);

            if (roll_move != 0 || pitch_move != 0)
                drone_status = ARDStatusFlying;
            
            break;
        }
            
            
        case ARDStatusFlying:
        {
#ifdef DEBUG_LOG_SERIAL
            Serial.println("__Flying");
#endif
            sprintf(data,"AT*PCMD=%d,1,%ld,%ld,%ld,%ld\r", seq++, roll_move, pitch_move, throttle_move, yaw_move);
            
            if (roll_move == 0 && pitch_move == 0)
                drone_status = ARDStatusHovering;
            
            break;
        }
    }
    
    // Send data over serial and pause a brief moment
    Serial.println(data);
#ifdef DEBUG_LOG_SERIAL
    // Some more delay so console won't get spammed
    delay(400);
#else
    delay(10);
#endif
    
#ifdef DEBUG_LED
    // Blink fast to give signal of being alive :)
    digitalWrite(LEDPIN, HIGH);
    delay(10);
    digitalWrite(LEDPIN, LOW);
#endif
}

void read_radio()
{
    // Reads signals from receiver
    // Tested with Multiplex M-Link RX-6 light
    
    // Only read 3 channels at once alternating to speed up the process since every 
    // read could in the worst case take up to RC_INPUT_TIMEOUT ms!
    switch (radio_read_seq)
    {
        case 0:
        {
            roll        = pulseIn(ROLLPIN, HIGH, RC_INPUT_TIMEOUT);
            throttle    = pulseIn(THROTTLEPIN, HIGH, RC_INPUT_TIMEOUT);
            aux_1       = pulseIn(AUX1PIN, HIGH, RC_INPUT_TIMEOUT);
            radio_read_seq = 1;
            
            break;
        }
            
        case 1:
        {
            pitch       = pulseIn(PITCHPIN, HIGH, RC_INPUT_TIMEOUT);
            yaw         = pulseIn(YAWPIN, HIGH, RC_INPUT_TIMEOUT);
            aux_2       = pulseIn(AUX2PIN, HIGH, RC_INPUT_TIMEOUT);
            radio_read_seq = 0;
            
            break;
        }
    }
    
    // If could not read radio , then sent command to hover drone
    if (roll == 0 || pitch == 0 || throttle == 0 || yaw == 0)
    {
        if (drone_status != ARDStatusGrounded)
        {
            if (roll == 0)
                roll = roll_mid;
            if (pitch == 0)
                pitch = pitch_mid;
            if (throttle == 0)
                throttle = throttle_mid;
            if (yaw == 0)
                yaw = yaw_mid;
        }
    }
    
    
    // Check ground/flying switch
    if (aux_1 > RC_INPUT_CENTER && aux_1 < 2500)
        flying_switch = 1; // flying
    
    if (aux_1 < RC_INPUT_CENTER)
        flying_switch = 0; // grounded
        
    // Check reset switch
    if (aux_2 > RC_INPUT_CENTER && aux_2 < 2500)
        reset_switch = 1; // flying
    
    if (aux_2 < RC_INPUT_CENTER)
        reset_switch = 0; // grounded
    
    
    // YAW movement
    // YAW Middle
    if (yaw < (yaw_mid + RC_DEADZONE_YAW) && yaw > (yaw_mid - RC_DEADZONE_YAW))
        yaw_move = 0;
    
    // YAW right
    if (yaw > (yaw_mid + RC_DEADZONE_YAW))
        yaw_move = float(yaw - RC_DEADZONE_YAW - yaw_mid) / float(yaw_step);
    
    // YAW left
    if (yaw < (yaw_mid - RC_DEADZONE_YAW))
        yaw_move = (float(yaw_mid - RC_DEADZONE_YAW - yaw) / float(yaw_step)) * float(-1);
    
    
    // Throttle movement
    // Throttle Middle
    if (throttle < (throttle_mid + RC_DEADZONE_THROTTLE) && throttle > (throttle - RC_DEADZONE_THROTTLE))
        throttle_move = 0;
    
    // Throttle up
    if (throttle > (throttle_mid + RC_DEADZONE_THROTTLE))
        throttle_move = float(throttle - RC_DEADZONE_THROTTLE - throttle_mid) / float(throttle_step);
    
    // Throttle down
    if (throttle < (throttle_mid - RC_DEADZONE_THROTTLE))
        throttle_move = (float(throttle_mid - RC_DEADZONE_THROTTLE - throttle) / float(throttle_step)) * float(-1);
    
    
    // Roll movement
    // Roll Middle
    if (roll < (roll_mid+RC_DEADZONE_ROLL) && roll > (roll_mid-RC_DEADZONE_ROLL))
        roll_move = 0;
    
    // Roll right
    if (roll > (roll_mid + RC_DEADZONE_ROLL))
        roll_move = float(roll - RC_DEADZONE_ROLL - roll_mid) / float(roll_step);
    
    // Roll left
    if (roll < (roll_mid - RC_DEADZONE_ROLL))
        roll_move = (float(roll_mid - RC_DEADZONE_ROLL - roll) / float(roll_step)) * float(-1);
    
    
    // Pitch movement
    // Pitch Middle
    if (pitch < (pitch_mid + RC_DEADZONE_PITCH) && pitch > (pitch_mid - RC_DEADZONE_PITCH) )
        pitch_move = 0;
    
    // Pitch up
    if (pitch > (pitch_mid + RC_DEADZONE_PITCH))
        pitch_move = float(pitch - RC_DEADZONE_PITCH - pitch_mid) / float(pitch_step);
    
    // Pitch down
    if (pitch < (pitch_mid - RC_DEADZONE_PITCH))
        pitch_move = (float(pitch_mid - RC_DEADZONE_ROLL - pitch) / float(pitch_step)) * float(-1);
}

void initialise_drone()
{
#ifdef DEBUG_LOG_SERIAL
    Serial.println("#_Initialising_Drone_!");
#endif

    // It seems the first commands send get cut off 
    // sometimes so send some dummy data.
    sprintf(data, "dummy_data_to_init_connection");

    switch (init_counter)
    {
        case 3: // Set drone ready to fly
            sprintf(data, "AT*CONFIG=%d\"general:navdata_demo\",\"TRUE\"\r", seq++);
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
            break;
            
        case 5: // Set yaw rate
            sprintf(data, "AT*CONFIG=%d,\"control:control_yaw\",\"%d\"\r", seq++, YAW_RATE);
            break;
            
        case 6: // Set throttle rate
            sprintf(data, "AT*CONFIG=%d,\"control:control_vz_max\",\"%d\"\r", seq++, THROTTLE_RATE);
            break;
            
        case 7: // Set pitch, roll rate
            sprintf(data, "AT*CONFIG=%d,\"control:euler_angle_max\",\"0.%d\"\r", seq++, PITCH_ROLL_RATE);
            break;
            
        case 8: // Set max altitude
            sprintf(data, "AT*CONFIG=%d,\"control:altitude_max\",\"%d\"\r", seq++, ALTITUDE_MAX);
            break;

            
        case 9: // Drone is ready , play LEDs
            sprintf(data, "AT*LED=%d,2,1073741824,5\r", seq++);
            break;

            
        case 10: // Set drone ready for flying
            init_drone = 0;
            drone_status = ARDStatusGrounded;
            break;
    }

    init_counter++;
    Serial.println(data);
    
#ifdef DEBUG_LED
    // Blink slowly >med__(long)< to give signal of init
    digitalWrite(LEDPIN, HIGH);
    delay(40);
    digitalWrite(LEDPIN, LOW);
    delay(60);
#endif
}
