#include "universal_mat.h"

universal::universal(const char *albedo_file, const char *normal_file, const char *metallic_file, const char *emissive_file, int emission_power)
    : albedo_texture(make_shared<image_texture>(albedo_file)), normal_texture(make_shared<image_texture>(normal_file)),
      metallic_texture(make_shared<image_texture>(metallic_file)), emissive_texture(make_shared<image_texture>(emissive_file)), emission_power(emission_power) {}

bool universal::scatter(const ray &r_in, const hit_record &rec,
                        color &attenuation, ray &scattered) const
{
    // Adjust normals with the normal map
    vec3 bitangent = cross(rec.normal, rec.tangent);
    matrix TBN = matrix(-rec.tangent, bitangent, rec.normal);
    vec3 normal = normal_texture->get_color_at(rec.u, rec.v) * 2 - vec3(1.0, 1.0, 1.0);
    normal = vec3(normal[0], normal[1], normal[2]);
    normal = TBN * normal;
    normal = unit_vector(normal);

    vec3 reflected = reflect(r_in.direction(), normal);
    // Fuzz is determined by metallic texture
    reflected = unit_vector(reflected) + ((color(1.0, 1.0, 1.0) - metallic_texture->get_color_at(rec.u, rec.v)) * random_unit_vector());
    scattered = ray(rec.p, reflected);
    attenuation = albedo_texture->get_color_at(rec.u, rec.v);
    return (dot(scattered.direction(), normal) > 0);
}

bool universal::emit(const hit_record &rec, color &emission) const
{
    emission = emissive_texture->get_color_at(rec.u, rec.v) * emission_power;
    return true;
}