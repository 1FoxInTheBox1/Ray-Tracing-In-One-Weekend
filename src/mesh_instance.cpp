#include "mesh_instance.h"

mesh_instance::mesh_instance(shared_ptr<mesh> mesh, point3 position, vec3 scale, quaternion rotation, shared_ptr<material> mat)
{
    make_triangles(mesh, position, scale, rotation, mat);
}

mesh_instance::mesh_instance(shared_ptr<mesh> mesh, point3 position, vec3 scale, vec3 rotation, shared_ptr<material> mat)
{
    make_triangles(mesh, position, scale, quaternion(rotation[0], rotation[1], rotation[2]), mat);
}

bool mesh_instance::hit(const ray &r, interval ray_t, hit_record &rec) const
{
    return triangles.hit(r, ray_t, rec);
}

aabb mesh_instance::get_bounding_box()
{
    return triangles.get_bounding_box();
}

// Adds each triangle in the mesh to the hittable_list
// This is done so that BVH construction can create nodes around
// individual triangles rather than the whole mesh
// Definitely a jank way of doing this, but it works for now
// TODO: Make this less jank
void mesh_instance::add_to_list(hittable_list &list)
{
    for (const auto &object : triangles.objects)
    {
        list.add(object);
    }
}

// Create triangles from the vertices list
// Note that texture coords are optional,
// and that we do not need to load normals
// since we calculate those ourselves
void mesh_instance::make_triangles(shared_ptr<mesh> mesh, point3 position, vec3 scale, quaternion rotation, shared_ptr<material> mat)
{
    std::vector<vec3> vertices;
    for (vec3 vertex : mesh->vertices)
    {
        vec3 new_vertex = vec3(vertex[0], vertex[1], vertex[2]);
        new_vertex *= scale;
        new_vertex = rotation_from_quaternion(rotation) * new_vertex;
        new_vertex += position;
        vertices.push_back(new_vertex);
    }

    for (int i = 0; i < vertices.size(); i += 3)
    {
        shared_ptr<triangle> tri;
        if (!mesh->tex_coords.empty())
        {
            tri = make_shared<triangle>(vertices[i], vertices[i + 1], vertices[i + 2],
                                        mesh->tex_coords[i], mesh->tex_coords[i + 1], mesh->tex_coords[i + 2], mat);
        }
        else
        {
            tri = make_shared<triangle>(vertices[i], vertices[i + 1], vertices[i + 2], mat);
        }
        triangles.add(tri);
    }
}