#include "sphere.h"

sphere::sphere(const point3 &center, double radius, shared_ptr<material> mat)
    : center(center), radius(fmax(0, radius)), mat(mat), hittable(center) {}

bool sphere::hit(const ray &r, interval ray_t, hit_record &rec) const
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
    // There was a comment here that said we ignore those tangential rays,
    // but this code doesn't seem to do that.
    // Noting it here just in case that ends up being important
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
    rec.tangent = unit_vector(cross(vec3(0, 1, 0), rec.p - center));
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;
    get_uv_coordinate(outward_normal, rec.u, rec.v);

    return true;
}

// Uses a spherical projection to turn a point on a sphere into
// a UV coordinate pair
void sphere::get_uv_coordinate(const point3 &p, double &u, double &v) const
{
    double phi = atan2(-p.z(), p.x()) + pi;
    double rho = acos(p.y());
    u = phi / (2 * pi);
    v = rho / pi;
}

aabb sphere::get_bounding_box()
{
    vec3 min = vec3(center.x() - radius, center.y() - radius, center.z() - radius);
    vec3 max = vec3(center.x() + radius, center.y() + radius, center.z() + radius);
    return aabb(min, max);
}