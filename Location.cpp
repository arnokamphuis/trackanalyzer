#include <stdlib.h>
#include <math.h>
#include "Location.h"

Location::Location(const double& lat, const double& lon, const double& ele, const long& t) :
        latitude(lat), longitude(lon), elevation(ele), time(t) {
    currentspeed = 0.0;
}

double Location::distance(const Location& l) const {

    double lat1 = latitude;
    double lon1 = longitude;
    double lat2 = l.latitude;
    double lon2 = l.longitude;

    double R = 6371000; // metres
    double phi1 =  lat1 * DEG2RAD;
    double phi2 = lat2 * DEG2RAD;
    double dphi = (lat2-lat1) * DEG2RAD;
    double dlambda = (lon2-lon1) * DEG2RAD;

    double a = sin(dphi/2.0) * sin(dphi/2.0) +
            cos(phi1) * cos(phi2) * sin(dlambda/2.0) * sin(dlambda/2.0);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    double d = R * c;

    return d;
}

double Location::speed(const Location& l) const {
    double dt = abs(l.getTime() - time);

    if (dt>0) {
        dt /= 1000;
        //        dt /= 1000 * 3600;
        double d = distance(l);
        double s = 3.6 * d / dt;
        if (s > 100) s = 0;
        return s;
    } else {
        return 0;
    }
}