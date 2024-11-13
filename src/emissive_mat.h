#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "material.h"
#include "texture.h"

// A class representing an emissive material
class emissive : public material
{
public:
    emissive(const color &albedo);
    emissive(const char *filename);

    bool scatter(const ray &r_in, const hit_record &rec,
                 color &attenuation, ray &scattered) const override;
    bool emit(const ray &r_in, const hit_record &rec,
              color &attenuation, ray &emitted) const override;

private:
    shared_ptr<texture> texture;
};

#endif