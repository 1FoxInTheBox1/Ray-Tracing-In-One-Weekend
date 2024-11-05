#include "dielectric_mat.h"

dielectric::dielectric(double refraction_index) : refraction_index(refraction_index) {}

// The Dielectric class represents a transparent material like glass or crystal
// It's scatter() function must then account for refraction and reflection
bool dielectric::scatter(const ray &r_in, const hit_record &rec, 
                            color &attenuation, ray &scattered) const
{
    attenuation = color(1.0, 1.0, 1.0);
    // The refraction index is the ratio of the object divided by the surrounding medium.
    // So when the ray is leaving the inside of the object we use 1/refraction_index.
    double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    // There are some cases where we cannot solve for refraction with Snell's Law
    // In those cases, we reflect. This usually happens inside objects and is called Total Internal Reflection
    if (cannot_refract || reflectance(cos_theta, ri) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, ri);

    scattered = ray(rec.p, direction);
    return true;
}