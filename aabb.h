#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"
#include "hittable.h"

// AABB stands for Axis-Aligned Bounding Box.
// They are used to implement BVH, a method for
// increasing the performance of the ray tracer.
class aabb
{
public:
    vec3 min, max;
    aabb() : min(inf_vector()), max(-inf_vector()) {}
    aabb(const vec3 &min, const vec3 &max) : min(min), max(max) {}

    void growToInclude(const vec3 &p)
    {
        min = min_vector(min, p);
        max = max_vector(max, p);
    }

    // This function is pulled straight from
    // Ray Tracing: The Next Week
    // because I got lazy and didn't want to do it myself
    // It uses the slab method
    bool hit(const ray &r, interval ray_t) const
    {
        const point3 &ray_orig = r.origin();
        const vec3 &ray_dir = r.direction();

        for (int axis = 0; axis < 3; axis++)
        {
            interval ax = interval(min.x(), max.x());
            if (axis == 1)
                ax = interval(min.y(), max.y());
            if (axis == 2)
                ax = interval(min.z(), max.z());
            const double adinv = 1.0 / ray_dir[axis];

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            if (t0 < t1)
            {
                if (t0 > ray_t.min)
                    ray_t.min = t0;
                if (t1 < ray_t.max)
                    ray_t.max = t1;
            }
            else
            {
                if (t1 > ray_t.min)
                    ray_t.min = t1;
                if (t0 < ray_t.max)
                    ray_t.max = t0;
            }

            if (ray_t.max < ray_t.min)
                return false;
        }
        return true;
    }
};

inline std::ostream &operator<<(std::ostream &out, const aabb &bounds)
{
    return out << "Min: " << bounds.min.e[0] << ' ' << bounds.min.e[1] << ' ' << bounds.min.e[2] << " Max: " << bounds.max.e[0] << ' ' << bounds.max.e[1] << ' ' << bounds.max.e[2];
}

#endif