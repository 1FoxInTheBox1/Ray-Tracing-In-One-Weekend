#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class aabb;
class material;

struct hit_record
{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t, u, v;
    bool front_face;

    void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        // Sets the hit record normal vector.
        // NOTE: 'outward_normal' is assumed to have unit length.
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    hittable() : pos(vec3(0, 0, 0)) {}
    hittable(vec3 position) : pos(position) {}
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;

    virtual aabb get_bounding_box() = 0;

    vec3 get_pos() const
    {
        return pos;
    }

private:
    vec3 pos;
};

#endif