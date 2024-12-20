#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"

class aabb;
class material;
class interval;

struct hit_record
{
    point3 p;
    vec3 normal;
    vec3 tangent;
    shared_ptr<material> mat;
    double t, u, v;
    bool front_face;
    bool from_emissive;

    void set_face_normal(const ray &r, const vec3 &outward_normal);
};

// A representation of an object that light rays can collide and interact with
class hittable
{
public:
    hittable();
    hittable(vec3 position);

    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
    virtual aabb get_bounding_box() = 0;
    vec3 get_pos() const;

private:
    vec3 pos;
};

#endif