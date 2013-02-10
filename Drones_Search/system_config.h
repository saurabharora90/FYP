/*
 *  user_config.h
 *
 *  Contains system defined constants
 */
 
#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "MapCoordinate.h"
#include "GPSCoordinate.h"

/**
* Grid assmumptions
*/

#define Col  20  //Predefining that the agent is searching a 20x20 grid.
#define Row  20

/**
* Algorithm constants
*/

#define tpLft  GPSCoordinate(1.434,103.643)         //topLeft GPS
#define tpRht  GPSCoordinate(1.434,103.700)         //topRight
#define	btLft  GPSCoordinate(1.339,103.643)         //bottomLeft
#define	btRht  GPSCoordinate(1.339,103.700)         //bottomRight

#define givenMap  MapCoordinate(10, 0)              //last known target's position
#define destMap   MapCoordinate(0,19)               //target's destination

/**
* WiFLY RN-131C constants
*/

#define Client_PORT           3000     //Server will send data to this port and client will listen at this port
#define Server_PORT           2000     //Server will listens on port 2000 and client will send data to this port.
#define ADHOC_NAME            "Drone_Server"
#define ADHOC_CHANNEL         10

/**
* Drone Specific Constants
*/

#define Drone_Type  "Server"

#endif
