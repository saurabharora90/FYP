#include "Grid.h"
//#include"GPSCoordinate.h"
//#include"MapCoordinate.h"

//Search Algorithm variables
GPSCoordinate curGPS; //Current GPS position of the drone.
GPSCoordinate targetGPS;
MapCoordinate curMap; //Current position of the drone on the map. Obtained by converting the GPS position to its MapCoordinate equivalent. grid.convertGPStoXY(curGPS.lat,curGPS.lon);
MapCoordinate targetMap;
