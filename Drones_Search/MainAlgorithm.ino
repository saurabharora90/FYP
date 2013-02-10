#include "Grid.h"
#include "system_config.h"

//Search Algorithm variables
GPSCoordinate curGPS; //Current GPS position of the drone.
GPSCoordinate targetGPS; //the next target to which it has to fly to

GPSCoordinate givenGPS;		//given last-known target position
GPSCoordinate destGPS;		//target destination

MapCoordinate curMap; //Current position of the drone on the map. Obtained by converting the GPS position to its MapCoordinate equivalent. grid.convertGPStoXY(curGPS.lat,curGPS.lon);
MapCoordinate targetMap;

Grid grid;
double PRCircleRadius;
double score[Row][Col];  //Can be replaced with a 2-d array if we know the number of rows and columns (which we are fixing to 20 as arduino doesnt support vectors)
double bestScore;
int maxTimeSinceLastVisit;
double preferredHeading;
double w_angle_modifier;
int timeSinceLastVisit[Row][Col]; //Can be replaced with a 2-d array if we know the number of rows and columns (which we are fixing to 20 as arduino doesnt support vectors)

void initSystem()
{
  grid.Begin(tpLft, tpRht, btLft, btRht);
  givenGPS = grid.convertXYtoGPS(givenMap.row, givenMap.col);
  destGPS = grid.convertXYtoGPS(destMap.row, destMap.col);
}
