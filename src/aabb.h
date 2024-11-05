#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"
#include "hittable.h"

class interval;

// AABB stands for Axis-Aligned Bounding Box.
// They are used to implement BVH trees, a method for
// increasing the performance of the ray tracer.
class aabb
{
public:
    vec3 min, max;
    aabb();
    aabb(const vec3 &min, const vec3 &max);

    void growToInclude(const vec3 &p);
    bool hit(const ray &r, interval ray_t) const;
};

inline std::ostream &operator<<(std::ostream &out, const aabb &bounds)
{
    return out << "Min: " << bounds.min.e[0] << ' ' << bounds.min.e[1] << ' ' << bounds.min.e[2] << " Max: " << bounds.max.e[0] << ' ' << bounds.max.e[1] << ' ' << bounds.max.e[2];
}

#endif