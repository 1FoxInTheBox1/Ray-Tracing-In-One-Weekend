#include "aabb.h"
#include "interval.h"

aabb::aabb() : min(inf_vector()), max(-inf_vector()) {}
aabb::aabb(const vec3 &min, const vec3 &max) : min(min), max(max) {}

// Expands the min and max of the aabb to include the point p
void aabb::growToInclude(const vec3 &p)
{
    min = min_vector(min, p);
    max = max_vector(max, p);
}

// Use the slab method to determine if a ray hits
bool aabb::hit(const ray &r, interval ray_t) const
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