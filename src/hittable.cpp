#include "hittable.h"

void hit_record::set_face_normal(const ray &r, const vec3 &outward_normal)
{
    // Sets the hit record normal vector.
    // NOTE: 'outward_normal' is assumed to have unit length.
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

hittable::hittable() : pos(vec3(0, 0, 0)) {}
hittable::hittable(vec3 position) : pos(position) {}

vec3 hittable::get_pos() const
{
    return pos;
}