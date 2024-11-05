#include "quaternion.h"

quaternion::quaternion() {};
quaternion::quaternion(double x, double y, double z, double w) : e{x, y, z, w} {};
// Create quaternion from euler angles
quaternion::quaternion(double roll, double pitch, double yaw) 
{
    double cr = cos(0.5 * roll);
    double sr = sin(0.5 * roll);
    double cp = cos(0.5 * pitch);
    double sp = sin(0.5 * pitch);
    double cy = cos(0.5 * yaw);
    double sy = sin(0.5 * yaw);

    e[0] = sr * cp * cy - cr * sp * sy;
    e[1] = cr * sp * cy + sr * cp * sy;
    e[2] = cr * cp * sy - sr * sp * cy;
    e[3] = cr * cp * cy + sr * sp * sy;
};

// Returns a new quaternion that is the normalized version 
// of the quaternion this was called on
quaternion quaternion::to_unit_quaternion()
{
    double theta = e[3] / 2.0;
    double s = sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + theta*theta);
    double nx = e[0] / s;
    double ny = e[1] / s;
    double nz = e[2] / s;
    double ntheta = theta / s;
    return quaternion(nx, ny, nz, ntheta);
}

// TODO: Kept having multiple dependency errors here, so this got moved to vec3
// Not really ideal, should try to fix this

// vec3 quaternion::to_euler_angles()
// {
//     vec3 angles;
//
//     // roll (x-axis rotation)
//     double sinr_cosp = 2 * (e[3] * e[0] + e[1] * e[2]);
//     double cosr_cosp = 1 - 2 * (e[0] * e[0] + e[1] * e[1]);
//     angles[0] = std::atan2(sinr_cosp, cosr_cosp);
//
//     // pitch (y-axis rotation)
//     double sinp = std::sqrt(1 + 2 * (e[3] * e[1] - e[0] * e[2]));
//     double cosp = std::sqrt(1 - 2 * (e[3] * e[1] - e[0] * e[2]));
//     angles[1] = 2 * std::atan2(sinp, cosp) - M_PI / 2;
//
//     // yaw (z-axis rotation)
//     double siny_cosp = 2 * (e[3] * e[2] + e[0] * e[1]);
//     double cosp_cosp = 1 - 2 * (e[1] * e[1] + e[2] * e[2]);
//     angles[2] = std::atan2(siny_cosp, cosp_cosp);
//
//     return angles;
// }