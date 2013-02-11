#include <Time.h>

#include "Grid.h"
#include "system_config.h"

#define E	2.718281828459045
#define H(X)   (fabs(X)<=(Pi/4.0)?pow((double)E,(double)(-(X)*(X)/2.0)):5.0*pow((double)E,(double)(-(X)*(X)/C)))
#define G(X)   (pow((double)E,(double)(-(X)*(X)/2.0/SIGMA/SIGMA)))

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
int pathMap[Row][Col]; ////The cells which are 1 in the path map array are the cells which are a part of the selected agent's search path

double startPosPercentage;	// percentage of path to start searching from
int startPosInt;		// actual member of path array
MapCoordinate startPosMap;	// calculated start search map coordinates
GPSCoordinate startPosGPS;	// calculated start search GPS coordinates

double deltaTAU;
double prevDeltaTAU;
double deltaNU;
double prevDeltaNU;
double MINTURNRADIUS;
double C_T;
double C_D;
double C;
double W_T;
double W_H;
double W_D;
double D_MAX;
double SIGMA;
double D_OPT;
double TAU;
double NU;
double LAMBDA;
double COMMRANGE;

void init_system()
{
  grid.Begin(tpLft, tpRht, btLft, btRht);
  givenGPS = grid.convertXYtoGPS(givenMap.row, givenMap.col); //last known target's position
  destGPS = grid.convertXYtoGPS(destMap.row, destMap.col); //target's destination.
  
  for(int i=0;i<Row;i++) //to set default values
  {
    for (int j=0;j<Col;j++)
    {
      timeSinceLastVisit[i][j] = 100;
      score[i][j] = 0;
      pathMap[i][j] = 0;
    }
  }
  
  MINTURNRADIUS = 1;  //in cells
  C_T = 100.0; //in sec
  C_D = 10.0;
  C = 2.0 * Pi * Pi / (32.0 * log(5.0) + Pi * Pi);
  D_MAX = sqrt((double)grid.nRow*grid.nRow+grid.nCol*grid.nCol);
  SIGMA = (D_MAX / sqrt(2.0*log(100.0)));
  W_T = 0.01;
  W_H = (C_T * W_T / (1 - pow(E,(-(Pi*Pi)/32.0)))/3.0);
  W_D = (C_T * W_T / (1 - pow(E,(- ( C_D * C_D )/(2.0 * SIGMA * SIGMA))))*2.0);
  D_OPT = (sqrt(2.0) * MINTURNRADIUS);
  TAU = 1000; //in terms of timeSincelastvisit
  NU = 0.02; // visit frequency in per sec
  COMMRANGE = 1400.0;
  PRCircleRadius = 360.0;
  double sum=0;
  LAMBDA =(sum/grid.traversalDist());
  deltaTAU=prevDeltaTAU=deltaNU=prevDeltaNU=0;
  w_angle_modifier=1.0;
  //preferredHeading=PropertyManager->getDoubleValue("attitude/heading-true-rad"); Need to calculate preferred heading as well.
  
  //Use the terrain data to generate the shortest path from last know target's destination to its final destination.
  //The cells which are 1 in the path map array are the cells which are a part of the selected agent's search path
  
  randomSeed(second());
  int shortestPathNum = random(0, 15); //Assuming the algorithm will generate 16 shortest paths and we have to randomlly select one to search along.
  
  //Calcuate shortest path and store it in pathmap. Also find the starting search position.
}

void update()
{
  static bool movedToGivenPosition = false;
  GPSCoordinate tempGPS;
  
  w_angle_modifier -= (w_angle_modifier<=1.01)?0:0.5;
  //preferredHeading=PropertyManager->getDoubleValue("attitude/heading-true-rad") * 180.0 / Pi; //Need to obtain preferredHeading
  
  maxTimeSinceLastVisit=0;
  
  for(int i=0;i<grid.nRow;i++)		// iterating through the rows
  {		
    for(int j=0;j<grid.nCol;j++)		// iterating through the columns
    {
        MapCoordinate tempMap = MapCoordinate(i,j);
	double distToIJ = curMap.distFrom(tempMap);			        // calculate the distance of agent to cell being examined
	
        tempGPS = grid.convertXYtoGPS(i,j);
        double headingToIJ = curGPS.headingTo(tempGPS);	                        // calculate the heading to that cell
	double angle_diff = preferredHeading - headingToIJ;			// calculate the difference between preferred heading and heading to cell, see whether it's feasible
	
        if(angle_diff>180)
	  angle_diff-=360.0;
	else if(angle_diff<-180)
	  angle_diff+=360.0;
			
        score[i][j]=W_T*timeSinceLastVisit[i][j] + w_angle_modifier*W_H*H(angle_diff*Pi/180.0) + W_D*G(distToIJ-D_OPT);		// scoring equation as described in paper

	// (KH 16/10/2011) code to introduce probability
	double startEndHeading;
	startEndHeading = givenGPS.headingTo(destGPS);			// (KH 29/01/2012) heading from known position to destination

	if (givenMap.distFrom(tempMap) < 5.0 && (givenGPS.headingTo(tempGPS) > (startEndHeading - 30.0) && givenGPS.headingTo(tempGPS) < (startEndHeading + 30.0)))
	  score[i][j] *= 1.2;
	else if (givenMap.distFrom(tempMap) < 10.0 && (givenGPS.headingTo(tempGPS) > (startEndHeading - 30.0) && givenGPS.headingTo(tempGPS) < (startEndHeading + 30.0)))
	  score[i][j] *= 1.1;

	// (KH 31/12/2011) code to consider ksp
	if (pathMap[i][j] == 1)
	  score[i][j] *=1.2;
			
	if(score[i][j]>bestScore) //if they are pretty much equal then make it random
	{	
          bestScore=score[i][j]; 
          targetMap.row=i;	
          targetMap.col=j;	// if we encountered a best scoring cell, we set the target to that cell
	} 
	
        timeSinceLastVisit[i][j]+=1;
	if(timeSinceLastVisit[i][j]>maxTimeSinceLastVisit)
	  maxTimeSinceLastVisit=timeSinceLastVisit[i][j];
    }
  }
  
  targetGPS = grid.convertXYtoGPS(targetMap.row,targetMap.col);
  
  //code to move to given position
  if (movedToGivenPosition != true) 
  {
    targetGPS.lat = startPosGPS.lat;
    targetGPS.lon = startPosGPS.lon;
    if (curMap.distFrom(startPosMap) < 1.0)
        movedToGivenPosition = true;
  }
}

void updateTimeSinceLastVisit()
{
  //assuming we have the current GPS retrieved 
  double lat = curGPS.lat;
  double lon = curGPS.lon;
  MapCoordinate tempMap = grid.convertGPStoXY(lat,lon);
  curMap = tempMap;
  if(0<=curMap.row && curMap.row<grid.nRow && 0<=curMap.col && curMap.col<grid.nCol)
      timeSinceLastVisit[curMap.row][curMap.col]=0;  //Reset timeSinceLastVisit to 0 for the grid point i'm currently on
}

void getCurrentGPS()
{
  //fetch curGPS from the GPS Module;
}
