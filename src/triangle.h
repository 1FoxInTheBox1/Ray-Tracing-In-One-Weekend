#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "interval.h"
#include "aabb.h"

const int epsilon = .00001;
const interval validRange = interval(-epsilon, epsilon);

class triangle : public hittable
{
public:
    triangle(const point3 &p0, const point3 &p1, const point3 &p2, shared_ptr<material> mat);

    bool hit (const ray &ray, interval ray_t, hit_record &rec) const override;
    aabb get_bounding_box();

private:
    point3 p0, p1, p2;
    shared_ptr<material> mat;
};

#endif