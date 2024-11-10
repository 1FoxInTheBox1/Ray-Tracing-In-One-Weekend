#ifndef MESH_H
#define MESH_H

#include "rtweekend.h"
#include "hittable_list.h"
#include "model_loader.h"
#include "lambertian_mat.h"

// A representation of the collection of vertices that make up a mesh
class mesh
{
public:
    std::vector<vec3> vertices;
    std::vector<vec3> tex_coords;
    
    mesh(const char *filename);
};

#endif