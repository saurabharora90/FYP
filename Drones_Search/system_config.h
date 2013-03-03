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
* Terrain Information
*/

double terrainInfo[20][20];  //this is required to calculate the shortest possible path taken by the target to get to destination and then searching along that path

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

WiFly wifly;
#define Client_PORT           3000     //Server will send data to this port and client will listen at this port
#define Server_PORT           2000     //Server will listens on port 2000 and client will send data to this port.
#define ADHOC_NAME            "Drone_Server"
#define ADHOC_CHANNEL         10

//Id for commands.
#define StartBeliefMapSend    "Starting sending of belief map"
#define EndBeliefMapSend      "Belief Map send complete"


/**
* Belief Information Exchange
*/

#define  Strip_Thickness      8        //We divide the grid into horizontal strips each 8 cells thick, i.e. the number of rows in Strip
#define  ColumnsInStrip       Col       //Currently we have a 20*20 Grid
#define  Strip_StartCellId(stripID)    stripId*Strip_Thickness*ColumnsInStrip
#define  Strip_EndCellId(stripId)      ((stripId+1)*Strip_Thickness*ColumnsInStrip)-1

/**
* Drone Specific Constants
*/

#define Drone_Type  "Server"

#endif
