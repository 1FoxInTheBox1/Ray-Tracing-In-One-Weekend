#ifndef QUATERNION_H
#define QUATERNION_H

#include "rtweekend.h"

class quaternion
{
public:
    double e[4];

    quaternion();
    quaternion(double x, double y, double z, double w);
    quaternion(double roll, double pitch, double yaw);

    inline quaternion operator-() const { return quaternion(-e[0], -e[1], -e[2], -e[3]); }
    inline double operator[](int i) const { return e[i]; }
    inline double &operator[](int i) { return e[i]; }

    inline quaternion conjugate() const { return quaternion(-e[0], -e[1], -e[2], e[3]); }
    
    quaternion to_unit_quaternion();
    // vec3 to_euler_angles();

private:
};

inline std::ostream &operator<<(std::ostream &out, const quaternion &q)
{
  return out << q.e[0] << ' ' << q.e[1] << ' ' << q.e[2] << ' ' << q.e[3];
}

#endif