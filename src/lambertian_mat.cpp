#include "lambertian_mat.h"

lambertian::lambertian(const color &albedo) : texture(make_shared<solid_color_texture>(albedo)) {}
lambertian::lambertian(const char *filename) : texture(make_shared<image_texture>(filename)) {}

// Lambertian materials scatter light (somewhat) randomly
// This scatter() function simulates that
bool lambertian::scatter(const ray &r_in, const hit_record &rec,
                         color &attenuation, ray &scattered) const
{
    auto scatter_direction = rec.normal + random_unit_vector();

    // Catch degenerate scatter directions
    // If the random unit vector is directly or almost directly opposite
    // the normal vector, we'll get a scatter direction of zero.
    // This code catches those degenerate directions.
    if (scatter_direction.near_zero())
    {
        scatter_direction = rec.normal;
    }

    scattered = ray(rec.p, scatter_direction);
    attenuation = texture->get_color_at(rec.u, rec.v);
    // attenuation = (rec.normal + vec3(0.0, 0.0, 1.0)) / 2;
    return true;
}