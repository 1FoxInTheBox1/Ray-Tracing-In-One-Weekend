#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// C++ Std Usings
using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

inline double random_double()
{
    // Return a random real in [0, 1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    // Returns a random real in [min, max)
    return min + (max - min) * random_double();
}

// TODO: Uncommenting this causes build errors
// Figure out how if there's a way to include important headers here
// or if I need to include each header individually
// Common Headers
// #include "vec3.h"
// #include "color.h"
// #include "interval.h"
// #include "ray.h"
// #include "aabb.h"

#endif