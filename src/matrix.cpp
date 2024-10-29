#include "matrix.h"

matrix::matrix() {}

// Creates a rotation matrix from the quaternion (x, y, z, w)
// Automatically normalizes the quaternion
matrix matrix::rotation_from_quaternion(double x, double y, double z, double w)
{
    double s = sqrt(x*x + y*y + z*z + w*w);
    double nx = x / s;
    double ny = y / s;
    double nz = z / s;
    double nw = w / s;

    // I found this conversion in Real-Time Rendering 4th edition
    matrix m = matrix();
    m.row_count = 4;
    m.col_count = 4;
    m(0, 0) = 1 - 2 * (ny*ny + nz*nz);
    m(0, 1) = 2 * (nx*ny - nw*nz);
    m(0, 2) = 2 * (nx*nz + nw*ny);
    m(0, 3) = 0;

    m(1, 0) = 2 * (nx*ny + nw*nz);
    m(1, 1) = 1 - 2 * (nx*nx + nz*nz);
    m(1, 2) = 2 * (ny*nz - nw*nx);
    m(1, 3) = 0;

    m(2, 0) = 2 * (nx*nz - nw*ny);
    m(2, 1) = 2 * (ny*nz + nw*nx);
    m(2, 2) = 1 - 2 * (nx*nx + ny*ny);
    m(2, 3) = 0;

    m(3, 0) = 0;
    m(3, 1) = 0;
    m(3, 2) = 0;
    m(3, 3) = 1;
    
    return m;
}

matrix matrix::rotation_from_quaternion(quaternion q)
{
    rotation_from_quaternion(q[0], q[1], q[2], q[3]);
}