#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:
    sphere(const point3 &center, double radius, shared_ptr<material> mat)
        : center(center), radius(fmax(0, radius)), mat(mat) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        // We can determine if a ray intersects the sphere 
        // with the quadratic formula. Since b=-2h
        // We can make some simplifications
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        // Remember: b = -2h
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        // If the discriminant is positive and non-zero, there are 2 solutions
        // (AKA the sphere hits the sphere in two locations)
        // If it is zero, there is 1 solution and the ray is tangential to the sphere
        // We say that these rays do not hit the sphere
        auto discriminant = h * h - a * c;
        if (discriminant < 0)
        {
            return false;
        }

        auto sqrtd = sqrt(discriminant);

        // Find nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // Save details of the collision to the hit_record
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};

#endif