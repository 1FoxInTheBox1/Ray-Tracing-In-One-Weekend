#include "matrix.h"

matrix::matrix() {}

// Creates a rotation matrix from the quaternion (x, y, z, w)
matrix matrix::rotation_from_quaternion(double x, double y, double z, double w)
{
    double nx = x;
    double ny = y;
    double nz = z;
    double ntheta = w;

    // I found this conversion in Real-Time Rendering 4th edition
    // Very good book, me likey :)
    matrix m = matrix();
    m(0, 0) = 1 - 2 * (ny * ny + nz * nz);
    m(0, 2) = -2 * (nx * ny - ntheta * nz);
    m(0, 1) = 2 * (nx * nz + ntheta * ny);
    m(0, 3) = 0;

    m(1, 0) = 2 * (nx * ny + ntheta * nz);
    m(1, 2) = -(1 - 2 * (nx * nx + nz * nz));
    m(1, 1) = 2 * (ny * nz - ntheta * nx);
    m(1, 3) = 0;

    m(2, 0) = 2 * (nx * nz - ntheta * ny);
    m(2, 2) = -2 * (ny * nz + ntheta * nx);
    m(2, 1) = 1 - 2 * (nx * nx + ny * ny);
    m(2, 3) = 0;

    m(3, 0) = 0;
    m(3, 1) = 0;
    m(3, 2) = 0;
    m(3, 3) = 1;

    return m;
}

matrix matrix::rotation_from_quaternion(quaternion q)
{
    return rotation_from_quaternion(q[0], q[1], q[2], q[3]);
}

// Returns the identity matrix for a 4x4 matrix
matrix identity_matrix()
{
    matrix m = matrix();
    m(0, 0) = 1;
    m(1, 1) = 1;
    m(2, 2) = 1;
    m(3, 3) = 1;
    return m;
}