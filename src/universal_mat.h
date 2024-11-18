#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include "material.h"
#include "texture.h"
#include "matrix.h"

// A class representing a material that can have varying properties based on supplied textures
class universal : public material
{
public:
    universal(const char *albedo_file, const char *normal_file, const char *metallic_file, const char *emissive_file, int emission_power);

    bool scatter(const ray &r_in, const hit_record &rec,
                 color &attenuation, ray &scattered) const override;
    bool emit(const hit_record &rec, color &attenuation) const override;

private:
    shared_ptr<texture> albedo_texture;
    shared_ptr<texture> normal_texture;
    shared_ptr<texture> metallic_texture;
    shared_ptr<texture> emissive_texture;
    int emission_power;
};

#endif