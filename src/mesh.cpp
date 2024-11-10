#include "mesh.h"

mesh::mesh(const char *filename)
{
    load_file(filename, vertices, tex_coords);
}