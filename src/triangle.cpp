#include "triangle.h"

triangle::triangle(const point3 &p0, const point3 &p1, const point3 &p2, shared_ptr<material> mat)
        : p0(p0), p1(p1), p2(p2), mat(mat), hittable((p0 + p1 + p2) / 3) {}

bool triangle::hit (const ray &ray, interval ray_t, hit_record &rec) const
{
    // TODO: store some values and check for speed

    // This code adapted from pseudocode from Realtime Rendering 4th Edition
    vec3 e1 = p1 - p0;
    vec3 e2 = p2 - p0;
    vec3 q = cross(ray.direction(), e2);
    double det = dot(e1, q);

    if (validRange.surrounds(det)) 
    {
        return false;
    }

    double f = 1.0/det;
    vec3 s = ray.origin() - p0;
    double u = f * dot(s, q);

    if (u < 0.0) 
    {
        return false;
    }

    vec3 r = cross(s, e1);
    double v = f * dot(ray.direction(), r);

    if (v < 0.0 || u + v > 1.0)
    {
        return false;
    }

    double t = f * dot(e2, r);
    // std::cout << ray_t.min << " - " << ray_t.max << " t = "<< t << "\n";
    if (!ray_t.surrounds(t)) {
        return false;
    }

    rec.t = t;
    rec.p = ray.at(rec.t);
    rec.set_face_normal(ray, unit_vector(cross(e1, e2)));
    //std::cout << unit_vector(cross(e1, e2)) << "\n";
    // std::cout << ray.direction() << " - " << rec.normal << "\n";
    // rec.normal = unit_vector(cross(e1, e2));
    rec.u = u;
    rec.v = v;
    rec.mat = mat;
    return true;
}

aabb triangle::get_bounding_box() 
{
    vec3 min_corner = vec3(std::min(std::min(p0.x(), p1.x()), p2.x()), 
                            std::min(std::min(p0.y(), p1.y()), p2.y()), 
                            std::min(std::min(p0.z(), p1.z()), p2.z()));
    vec3 max_corner = vec3(std::max(std::max(p0.x(), p1.x()), p2.x()), 
                            std::max(std::max(p0.y(), p1.y()), p2.y()), 
                            std::max(std::max(p0.z(), p1.z()), p2.z()));
    return aabb(min_corner, max_corner);
}