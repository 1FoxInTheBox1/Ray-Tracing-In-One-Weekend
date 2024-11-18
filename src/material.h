#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "rtweekend.h"
#include "color.h"
#include "ray.h"

// An abstract class representing an object's material properties
class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;

    virtual bool emit(const hit_record &rec, color &emission) const
    {
        return false;
    }
};

#endif