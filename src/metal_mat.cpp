#include "metal_mat.h"

metal::metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

// Metallic materials reflect light according to their "fuzz" factor
// Less fuzz = clearer reflections
bool metal::scatter(const ray &r_in, const hit_record &rec,
                    color &attenuation, ray &scattered) const
{
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    // We apply a slight randomization to the reflected vector to
    // blur reflections a bit (or a lot, depending on the fuzz).
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = ray(rec.p, reflected);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
}