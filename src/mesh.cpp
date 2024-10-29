#include "mesh.h"

mesh::mesh(const char *filename, point3 position, vec3 scale, quaternion rotation, shared_ptr<material> mat) 
{
    load_file(filename, position, scale, rotation, triangles, mat);
}

mesh::mesh(const char *filename, point3 position, shared_ptr<material> mat) 
{
    load_file(filename, position, vec3(1, 1, 1), quaternion(1, 0, 0, 0), triangles, mat);
}

bool mesh::hit(const ray &r, interval ray_t, hit_record &rec) const
{
    return triangles.hit(r, ray_t, rec);
}

aabb mesh::get_bounding_box()
{
    return triangles.get_bounding_box();
}

void mesh::add_to_list(hittable_list &list)
{
    for (const auto &object : triangles.objects)
    {
        list.add(object);
    }
}