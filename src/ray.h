#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
    ray();
    ray(const point3 &origin, const point3 &direction);

    // Ask Anthony about these
    // const point3 &origin() const { return orig; }
    // const point3 &direction() const { return dir; }
    const point3 &origin() const;
    const point3 &direction() const;
    vec3 at(double t) const;

private:
    point3 orig;
    point3 dir;
};

#endif