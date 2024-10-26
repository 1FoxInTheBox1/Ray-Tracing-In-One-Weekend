#ifndef MESH_H
#define MESH_H

#include "hittable_list.h"
#include "model_loader.h"

class mesh : public hittable
{
public:
    mesh(const char *filename, point3 position, vec3 scale, shared_ptr<material> mat);
    mesh(const char *filename, point3 position, shared_ptr<material> mat);

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;
    aabb get_bounding_box();
    void add_to_list(hittable_list &list);
private:
    hittable_list triangles;
};

#endif