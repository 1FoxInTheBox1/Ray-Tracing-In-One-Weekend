#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// A class representing a ray,
// mainly used to represent light rays
class ray
{
public:
    ray();
    ray(const point3 &origin, const point3 &direction);

    const point3 &origin() const;
    const point3 &direction() const;
    vec3 at(double t) const;

private:
    point3 orig;
    point3 dir;
};

#endif