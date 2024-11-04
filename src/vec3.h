#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"
#include "quaternion.h"
#include "matrix.h"

inline double random_double();
inline double random_double(double min, double max);

class vec3
{

public:
  double e[3];

  vec3();
  vec3(double e0, double e1, double e2);
  inline double x() const { return e[0]; }
  inline double y() const { return e[1]; }
  inline double z() const { return e[2]; }

  inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  inline double operator[](int i) const { return e[i]; }
  inline double &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &v);
  vec3 &operator*=(const vec3 &v);
  vec3 &operator*=(double t);
  vec3 &operator/=(double t);

  double length() const;
  double length_squared() const;
  bool near_zero() const;
  void rotate(const quaternion &q);

  static vec3 random()
  {
    return vec3(random_double(), random_double(), random_double());
  }

  static vec3 random(double min, double max)
  {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
  }
};

// define alias
using point3 = vec3;

// Utility Functions
vec3 inf_vector();
vec3 min_vector(const vec3 &a, const vec3 &b);
vec3 max_vector(const vec3 &a, const vec3 &b);
vec3 to_euler_angles(const quaternion &q);

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v)
{
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t)
{
  return t * v;
}

inline vec3 operator*(const matrix &m, const vec3 &v)
{
    vec3 result;
    for (int i = 0; i < 3; i++)
    {
        result[i] = m(i, 0) * v[0] + m(i, 1) * v[1] + m(i, 2) * v[2];
    }
    return result;
}

inline vec3 operator/(const vec3 &v, double t)
{
  return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v)
{
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v)
{
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3 &v)
{
  return v / v.length();
}

inline vec3 random_in_unit_sphere()
{
  while (true)
  {
    auto p = vec3::random(-1, 1);
    if (p.length_squared() < 1)
      return p;
  }
}

inline vec3 random_unit_vector()
{
  return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3 &normal)
{
  vec3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0.0)
    return on_unit_sphere;
  else
    return -on_unit_sphere;
}

inline vec3 random_in_unit_disk()
{
  while (true)
  {
    auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.length_squared() < 1)
      return p;
  }
}

// Determines the direction of a ray reflected across a surface
// with normal vector n
inline vec3 reflect(const vec3 &v, const vec3 &n)
{
  return v - 2 * dot(v, n) * n;
}

// Determines the direction of a refracted ray according to Snell's Law
inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
  auto cos_theta = fmin(dot(-uv, n), 1.0);
  vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

#endif