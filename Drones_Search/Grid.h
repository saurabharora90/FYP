#ifndef GRID
#define GRID

#include "MapCoordinate.h"
#include "GPSCoordinate.h"
#include <vector>

#define Col  20  //Predefining that the agent is searching a 20x20 grid.
#define Row  20
using namespace std;

class Grid
{
private:
	GPSCoordinate topLeft;
	GPSCoordinate topRight;
	GPSCoordinate bottomLeft;
	GPSCoordinate bottomRight;
	//vector< vector<GPSCoordinate> > centerGPSCoords;  //Equivalent to a double dimensional array? If we know that for each push_back(vector) how many vectors will be there.
        GPSCoordinate centerGPSCoords[Row][Col];

public:
	int nCol;
	int nRow;
	Grid(GPSCoordinate TL, GPSCoordinate TR, GPSCoordinate BL, GPSCoordinate BR):
	  topLeft(TL),topRight(TR),bottomLeft(BL),bottomRight(BR),nCol(Col),nRow(Row)
	{
		for(int i=0;i<nRow;i++)
		{
			//vector<GPSCoordinate> tempVec;
                        GPSCoordinate tempVec[Col];
			GPSCoordinate projectionOnLeft = topLeft.coordAtRatioFrom(bottomLeft,(i+0.5)/nRow);
			GPSCoordinate projectionOnRight = topRight.coordAtRatioFrom(bottomRight,(i+0.5)/nRow);
			for(int j=0;j<nCol;j++)
                        {
				//tempVec.push_back(projectionOnLeft.coordAtRatioFrom(projectionOnRight,(j+0.5)/nCol));
                                tempVec[j]=projectionOnLeft.coordAtRatioFrom(projectionOnRight,(j+0.5)/nCol);
                                centerGPSCoords[i][j] = tempVec[j];
                        }
			//centerGPSCoords.push_back(tempVec);
		}
	}
	Grid():	topLeft(0,0),topRight(0,0),bottomLeft(0,0),bottomRight(0,0),nCol(0),nRow(0){}

	MapCoordinate convertGPStoXY(double latitude,double longitude)
	{
		int row, col;
		double sLeft = (topLeft.lon-bottomLeft.lon)/(topLeft.lat-bottomLeft.lat);
		double sTop = (topLeft.lon-topRight.lon)/(topLeft.lat-topRight.lat);
		if(sLeft==0)
		{
			col = (int) ((longitude-topLeft.lon)/(topRight.lon-topLeft.lon) * nCol);
			row = (int) ((latitude-topLeft.lat)/(bottomLeft.lat-topLeft.lat) * nRow);
		}
		else
		{
			double latProjTop = (sLeft*latitude - sTop*topRight.lat - longitude + topRight.lon)/(sLeft-sTop);
			double lonProjTop = sLeft*(latProjTop-latitude) + longitude;
			GPSCoordinate temp(latProjTop, lonProjTop);
			//col = (int) (topLeft.distFrom(GPSCoordinate(latProjTop,lonProjTop))/topRight.distFrom(topLeft)*nCol);
			col = (int) (topLeft.distFrom(temp)/topRight.distFrom(topLeft)*nCol);

			double latProjLeft = (longitude - bottomLeft.lon - sTop*latitude + sLeft*bottomLeft.lat)/(sLeft-sTop);
			double lonProjLeft = sTop*(latProjLeft-latitude) + longitude;
			GPSCoordinate temp1(latProjLeft, lonProjLeft);
			//row = (int) (topLeft.distFrom(GPSCoordinate(latProjLeft,lonProjLeft))/bottomLeft.distFrom(topLeft)*nRow);
			row = (int) (topLeft.distFrom(temp1)/bottomLeft.distFrom(topLeft)*nRow);
		}
		return MapCoordinate(row,col);
	}

	GPSCoordinate convertXYtoGPS(int row, int col)
	{
		return centerGPSCoords[row][col];
	}

	double area() { return topLeft.distFrom(topRight) * topLeft.distFrom(bottomLeft);	}

	double traversalDist() { return centerGPSCoords[0][0].distFrom(centerGPSCoords[0][nCol-1])*nRow
		+centerGPSCoords[0][0].distFrom(centerGPSCoords[nRow-1][0]);}

};

#endif
