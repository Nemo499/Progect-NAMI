#pragma once
#include <GeographicLib/Constants.hpp>
#include <GeographicLib/Geocentric.hpp>


class GPSPoint
{
public:
    static void convertWGStoGeocentric(double lat, double lon, double ele, double &x, double &y, double &z)
    {
        converter.Forward(lat, lon, ele, x, y, z);
    }

    static void convertGeocentricToWGS(double x, double y, double z, double &lat, double &lon, double &ele)
    {
        converter.Reverse(x, y, z, lat, lon, ele);
    }

    inline const static GeographicLib::Geocentric converter = GeographicLib::Geocentric(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
};
