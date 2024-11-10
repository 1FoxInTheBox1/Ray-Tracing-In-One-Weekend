#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "hittable_list.h"
#include "mesh.h"

// A representation of the collection of triangles that make up a mesh instance
class mesh_instance : public hittable
{
public:
    mesh_instance(shared_ptr<mesh> mesh, point3 position, vec3 scale, quaternion rotation, shared_ptr<material> mat);
    mesh_instance(shared_ptr<mesh> mesh, point3 position, vec3 scale, vec3 rotation, shared_ptr<material> mat);

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;
    aabb get_bounding_box();
    void add_to_list(hittable_list &list);

private:
    hittable_list triangles;

    void make_triangles(shared_ptr<mesh> mesh, point3 position, vec3 scale, quaternion rotation, shared_ptr<material> mat);
};

#endif