#include "universal_mat.h"

universal::universal(const char *albedo_file, const char *normal_file, const char *metallic_file, const char *emissive_file, int emission_power)
    : albedo_texture(make_shared<image_texture>(albedo_file)), normal_texture(make_shared<image_texture>(normal_file)),
      metallic_texture(make_shared<image_texture>(metallic_file)), emissive_texture(make_shared<image_texture>(emissive_file)), emission_power(emission_power) {}

bool universal::scatter(const ray &r_in, const hit_record &rec,
                        color &attenuation, ray &scattered) const
{
    // // TODO: I think rays are getting sent inside the object, need to fix this
    // // Adjust normals with the normal map
    vec3 bitangent = cross(rec.normal, rec.tangent);
    matrix TBN = matrix(-rec.tangent, bitangent, rec.normal);
    vec3 normal = normal_texture->get_color_at(rec.u, rec.v) * 2 - vec3(1.0, 1.0, 1.0);
    normal = vec3(normal[0], normal[1], normal[2]);
    normal = TBN * normal;
    normal = unit_vector(normal);

    vec3 reflected = reflect(r_in.direction(), normal);
    // We apply a slight randomization to the reflected vector to
    // blur reflections a bit (or a lot, depending on the fuzz).
    reflected = unit_vector(reflected) + ((color(1.0, 1.0, 1.0) - metallic_texture->get_color_at(rec.u, rec.v)) * random_unit_vector());
    scattered = ray(rec.p, reflected);
    attenuation = albedo_texture->get_color_at(rec.u, rec.v);
    return (dot(scattered.direction(), normal) > 0);

    // auto scatter_direction = rec.normal + random_unit_vector();

    // // Catch degenerate scatter directions
    // // If the random unit vector is directly or almost directly opposite
    // // the normal vector, we'll get a scatter direction of zero.
    // // This code catches those degenerate directions.
    // if (scatter_direction.near_zero())
    // {
    //     scatter_direction = rec.normal;
    // }

    // scattered = ray(rec.p, scatter_direction);
    // attenuation = (normal + vec3(0.0, 0.0, 1.0)) / 2;
    // return true;
}

bool universal::emit(const hit_record &rec, color &emission) const
{
    emission = emissive_texture->get_color_at(rec.u, rec.v) * emission_power;
    return true;
}