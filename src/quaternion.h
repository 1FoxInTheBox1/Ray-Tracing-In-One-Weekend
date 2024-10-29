#ifndef QUATERNION_H
#define QUATERNION_H

class quaternion
{
public:
    double e[4];

    quaternion();
    quaternion(double x, double y, double z, double w);

    inline quaternion operator-() const { return quaternion(-e[0], -e[1], -e[2], -e[3]); }
    inline double operator[](int i) const { return e[i]; }
    inline double &operator[](int i) { return e[i]; }

    inline quaternion conjugate() const { return quaternion(-e[0], -e[1], -e[2], e[3]); }

private:
};

#endif