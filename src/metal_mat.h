#ifndef METAL_H
#define METAL_H

#include "material.h"

// A class representing metallic, reflective materials
class metal : public material
{
public:
    metal(const color &albedo, double fuzz);

    bool scatter(const ray &r_in, const hit_record &rec, 
                color &attenuation, ray &scattered) const override;

private:
    color albedo;
    double fuzz;
};

#endif