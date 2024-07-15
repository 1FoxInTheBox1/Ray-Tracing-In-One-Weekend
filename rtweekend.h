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
inline double degrees_to_radian(double degrees) {
    return degrees * pi / 180;
}

inline double random_double() {
    // Return a random real in [0, 1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min, max)
    return min + (max-min)*random_double();
}

// Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif