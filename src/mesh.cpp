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

// Adds each triangle in the mesh to the hittable_list
// This is done so that BVH construction can create nodes around
// individual triangles rather than the whole mesh
// Definitely a jank way of doing this, but it works for now
// TODO: Make this less jank
void mesh::add_to_list(hittable_list &list)
{
    for (const auto &object : triangles.objects)
    {
        list.add(object);
    }
}