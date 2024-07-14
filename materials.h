#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"

#define drand48() ((double)rand()/RAND_MAX)

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (dot(p,p) >= 1.0);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n) {
    return 2.0*dot(v,n)*n - v;
}

class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

class metal : public material {
    public:
        metal(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            // print_vec3(r_in.direction());
            // std::cout << " --- ";
            // std::cout << 2.0 * dot(r_in.direction(),rec.normal) << " --- ";
            // print_vec3(rec.normal);
            // std::cout << " --- ";
            // print_vec3(reflected);
            // std::cout << "\n";
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

        vec3 albedo;
};

#endif