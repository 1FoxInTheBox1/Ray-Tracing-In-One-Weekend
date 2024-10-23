#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"
#include "rtweekend.h"

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attnuation, ray &scattered) const
    {
        return false;
    }
};

// A class representing a diffuse material
class lambertian : public material
{
public:
    lambertian(const color &albedo) : texture(make_shared<solid_color_texture>(albedo)) {}
    lambertian(const char *filename) : texture(make_shared<image_texture>(filename)) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
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
        return true;
    }

private:
    shared_ptr<texture> texture;
};

// A class representing metallic, reflective materials
class metal : public material
{
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        // We apply a slight randomization to the reflected vector to 
        // blur reflections a bit (or a lot, depending on the fuzz).
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

// A class representing transparent materials like glass
class dielectric : public material
{
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
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

private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index)
    {
        // Use Schlick's approximation for reflectance
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif