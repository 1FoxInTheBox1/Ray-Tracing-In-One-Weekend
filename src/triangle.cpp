#include "triangle.h"

triangle::triangle(const point3 &p0, const point3 &p1, const point3 &p2, shared_ptr<material> mat)
    : p0(p0), p1(p1), p2(p2), mat(mat),
      tex_coord0(vec3(0, 0, 0)), tex_coord1(vec3(1, 0, 0)), tex_coord2(vec3(0, 1, 0)),
      hittable((p0 + p1 + p2) / 3), e1(p1 - p0), e2(p2 - p0), normal(unit_vector(cross(e1, e2))) {}

triangle::triangle(const point3 &p0, const point3 &p1, const point3 &p2,
                   const point3 &tex_coord0, const point3 &tex_coord1, const point3 &tex_coord2,
                   shared_ptr<material> mat)
    : p0(p0), p1(p1), p2(p2),
      tex_coord0(min_vector(max_vector(tex_coord0, vec3(0, 0, 0)), vec3(1, 1, 1))),
      tex_coord1(min_vector(max_vector(tex_coord1, vec3(0, 0, 0)), vec3(1, 1, 1))),
      tex_coord2(min_vector(max_vector(tex_coord2, vec3(0, 0, 0)), vec3(1, 1, 1))),
      mat(mat), hittable((p0 + p1 + p2) / 3), e1(p1 - p0), e2(p2 - p0), normal(unit_vector(cross(e1, e2))) {}

bool triangle::hit(const ray &ray, interval ray_t, hit_record &rec) const
{
    // This code adapted from pseudocode from Realtime Rendering 4th Edition
    // It utilizes barycentric coordinates to determine the intersection point
    // e1, e2, and normal are calculated when the triangle is created
    vec3 q = cross(ray.direction(), e2);
    double det = dot(e1, q);

    if (validRange.surrounds(det))
    {
        return false;
    }

    double f = 1.0 / det;
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
    if (!ray_t.surrounds(t))
    {
        return false;
    }

    rec.t = t;
    rec.p = ray.at(rec.t);
    rec.tangent = unit_vector(e1);
    rec.set_face_normal(ray, normal);
    rec.u = (1 - u - v) * tex_coord0.x() + u * tex_coord1.x() + v * tex_coord2.x();
    rec.v = (1 - u - v) * tex_coord0.y() + u * tex_coord1.y() + v * tex_coord2.y();
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