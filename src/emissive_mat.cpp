#include "emissive_mat.h"

emissive::emissive(const color &albedo) : texture(make_shared<solid_color_texture>(albedo)) {}
emissive::emissive(const char *filename) : texture(make_shared<image_texture>(filename)) {}

// Emissive materials always render at full brightness and are not affected by lighting
bool emissive::scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scattered) const
{
    return false;
}

bool emissive::emit(
    const ray &r_in, const hit_record &rec, color &attenuation, ray &emitted) const
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

    emitted = ray(rec.p, scatter_direction);
    attenuation = texture->get_color_at(rec.u, rec.v);
    return true;
}