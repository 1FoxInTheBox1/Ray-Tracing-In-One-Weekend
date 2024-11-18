#include "emissive_mat.h"

emissive::emissive(const color &albedo) : texture(make_shared<solid_color_texture>(albedo)) {}
emissive::emissive(const char *filename) : texture(make_shared<image_texture>(filename)) {}

// Emissive materials always render at full brightness and are not affected by lighting
bool emissive::scatter(const ray &r_in, const hit_record &rec,
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
    attenuation = color(0, 0, 0);
    return true;
}

bool emissive::emit(const hit_record &rec, color &emission) const
{
    emission = texture->get_color_at(rec.u, rec.v);
    return true;
}