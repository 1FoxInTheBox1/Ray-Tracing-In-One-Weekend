#ifndef MATRIX_H
#define MATRIX_H

#include "rtweekend.h"
#include "quaternion.h"

#include <vector>

// A representation of a 4x4 matrix.
class matrix
{
public:
    matrix();
    inline double operator()(int row, int col) const { return data[row][col]; }
    inline double &operator()(int row, int col) { return data[row][col]; }

    // Creates a rotation matrix from the quaternion (x, y, z, w)
    // Automatically normalizes the quaternion
    static matrix rotation_from_quaternion(double x, double y, double z, double w);
    static matrix rotation_from_quaternion(quaternion q);

private:
    double data[4][4];
};

matrix identity_matrix();

inline std::ostream &operator<<(std::ostream &out, const matrix &m)
{
    return out << m(0, 0) << ' ' << m(0, 1) << ' ' << m(0, 2) << ' ' << m(0, 3) << '\n'
               << m(1, 0) << ' ' << m(1, 1) << ' ' << m(1, 2) << ' ' << m(1, 3) << '\n'
               << m(2, 0) << ' ' << m(2, 1) << ' ' << m(2, 2) << ' ' << m(2, 3) << '\n'
               << m(3, 0) << ' ' << m(3, 1) << ' ' << m(3, 2) << ' ' << m(3, 3);
    // return out << "test";
}

#endif