#include "ray.h"

ray::ray() {}
ray::ray(const point3 &origin, const point3 &direction) : orig(origin), dir(direction) {}

const point3 &ray::origin() const { return orig; }
const point3 &ray::direction() const { return dir; }
vec3 ray::at(double t) const { return orig + dir * t; }