#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "interval.h"
#include "aabb.h"

class sphere : public hittable
{
public:
    sphere(const point3 &center, double radius, shared_ptr<material> mat);

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;
    void get_uv_coordinate(const point3 &p, double &u, double &v) const;
    aabb get_bounding_box();

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

#endif