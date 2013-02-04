#ifndef GPSCOORDINATE
#define GPSCOORDINATE

#include <math.h>
#define Pi 3.141592653589793
using namespace std;

class GPSCoordinate
{
	const double DEGtoRAD;
public:
	double lat;
	double lon;
	
	GPSCoordinate(double latVal, double lonVal):lat(latVal),lon(lonVal),DEGtoRAD(Pi/180.0){}
	GPSCoordinate():lat(0),lon(0),DEGtoRAD(Pi/180.0){}

	double distFrom(GPSCoordinate &other) const
	{
		return sqrt((other.lon-lon)*(other.lon-lon)+(other.lat-lat)*(other.lat-lat));
	}

	double distInFeetFrom(GPSCoordinate &other) const
	{
		// Haversine formula
		double dLat = (other.lat-lat) * DEGtoRAD;
		double dLon = (other.lon-lon) * DEGtoRAD; 
		return 6371 * 2 * asin(sqrt(sin(dLat/2) * sin(dLat/2) + cos(lat * DEGtoRAD) * cos(other.lat * DEGtoRAD) * sin(dLon/2) * sin(dLon/2))) * 3280.8399;
	}

	bool operator==(GPSCoordinate &other) const
	{
		return (lat==other.lat && lon==other.lon);
	}
	GPSCoordinate & operator=(const GPSCoordinate &other)
	{
		lat = other.lat;
		lon = other.lon;
		return *this;
	}
	GPSCoordinate coordAtRatioFrom(GPSCoordinate &other, double ratio) const
	{
		GPSCoordinate result;
		result.lat = lat+ratio*(other.lat-lat);
		result.lon = lon+ratio*(other.lon-lon);
		return result;
	}
	
	GPSCoordinate coordAtDistAndHeading(double distInFeet, double headingDeg)
	{
		double angleForPolarCoordRad = Pi/180.0 * fmod(90.0 + 360.0-headingDeg , 360.0);
		GPSCoordinate result;
		double distInGPS = (distInFeet/3.28084)/2000 * 0.018;
		result.lat = lat + distInGPS*sin(angleForPolarCoordRad);
		result.lon = lon + distInGPS*cos(angleForPolarCoordRad);
		return result;
	}

	double headingTo(GPSCoordinate &other) const
	{
		double hypotenuse_distance = sqrt(pow(lat - other.lat, 2) + pow(lon - other.lon, 2));
		double adjacent_distance = fabs(lat - other.lat);
		double theAngle = acos(adjacent_distance/hypotenuse_distance) * 180.0 / Pi;

		if (other.lon > lon)
		{
			if (other.lat < lat)
				theAngle = 180 - theAngle;
		}
		else
		{	
			if (other.lat < lat)
				theAngle += 180.0;
			else
				theAngle = 360.0 - theAngle;
		}
		return(theAngle);
	}
};
#endif
