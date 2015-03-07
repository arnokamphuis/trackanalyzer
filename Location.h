#ifndef __LOCATION_H__
#define __LOCATION_H__


#define DEG2RAD 0.01745329251994329576923690768489

class Location {
private:
protected:
    double latitude;
    double longitude;
    double elevation;
    long   time;
    double currentspeed;
public:
    Location(const double& lat, const double& lon, const double& ele, const long& t);

    double distance(const Location& l) const;
    double speed(const Location& l) const;

    inline void setSpeed(double s) {currentspeed = s;}
    inline long getTime() const { return time; }

};

#endif // __LOCATION_H__