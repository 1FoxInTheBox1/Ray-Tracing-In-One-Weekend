#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(vec3 cen, float r) : center(cen), radius(r) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        vec3 center;
        float radius;
    
    private:
        void update_hit_record(float t, ray r, hit_record& rec) const;
};

void sphere::update_hit_record(float t, ray r, hit_record& rec) const {
    rec.t = t;
    rec.p = r.point_at_parameter(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
}

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = center - r.origin();
    float a = r.direction().squared_length();
    float b = dot(r.direction(), oc);
    float c = oc.squared_length() - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (b - sqrt(discriminant)/a);
        if (temp < t_max && temp > t_min) {
            update_hit_record(temp, r, rec);
            return true;
        }
        temp = (b - sqrt(discriminant)/a);
        if (temp < t_max && temp > t_min) {
            update_hit_record(temp, r, rec);
            return true;
        }
    }
    return false;
}

#endif