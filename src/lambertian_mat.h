#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "texture.h"

// A class representing a diffuse material
class lambertian : public material
{
public:
    lambertian(const color &albedo);
    lambertian(const char *filename);

    bool scatter(const ray &r_in, const hit_record &rec,
                 color &attenuation, ray &scattered) const override;

private:
    shared_ptr<texture> texture;
};

#endif