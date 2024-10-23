#include "vec3.h"

vec3::vec3() {}
vec3::vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

vec3 &vec3::operator+=(const vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

vec3 &vec3::operator*=(double t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

vec3 &vec3::operator/=(double t)
{
    return *this *= 1 / t;
}

double vec3::length() const
{
    return sqrt(length_squared());
}

double vec3::length_squared() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

bool vec3::near_zero() const
{
    auto s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
}

vec3 inf_vector() {
  return vec3(infinity, infinity, infinity);
}

vec3 min_vector(const vec3 &a, const vec3 &b) {
  vec3 result = vec3();
  a.x() < b.x() ? result[0] = a.x() : result[0] = b.x();
  a.y() < b.y() ? result[1] = a.y() : result[1] = b.y();
  a.z() < b.z() ? result[2] = a.z() : result[2] = b.z();
  return result;
}

vec3 max_vector(const vec3 &a, const vec3 &b) {
  vec3 result = vec3();
  a.x() > b.x() ? result[0] = a.x() : result[0] = b.x();
  a.y() > b.y() ? result[1] = a.y() : result[1] = b.y();
  a.z() > b.z() ? result[2] = a.z() : result[2] = b.z();
  return result;
}