#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

// A handy utility class to represent an interval between two real numbers
class interval
{
public:
    double min, max;

    interval();
    interval(double min, double max);

    double size() const;
    bool contains(double x) const;
    bool surrounds(double x) const;

    double clamp(double x) const;

    static const interval empty, universe;
};

const interval empty = interval(+infinity, -infinity);
const interval universe = interval(-infinity, +infinity);

#endif