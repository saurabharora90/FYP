#ifndef MAPCOORDINATE
#define MAPCOORDINATE

#include <math.h>
using namespace std;

class MapCoordinate
{
public:
	int row;
	int col;
	
	MapCoordinate(int r, int c):row(r),col(c){}
	MapCoordinate():row(0),col(0){}

	double distFrom(MapCoordinate &other) const
	{
		return sqrt((double)(other.col-col)*(other.col-col)+(other.row-row)*(other.row-row));
	}
	bool operator==(MapCoordinate &other) const
	{
		return (row==other.row && col==other.col);
	}
	MapCoordinate & operator=(MapCoordinate &other)
	{
		row = other.row;
		col = other.col;
		return *this;
	}
};
#endif
