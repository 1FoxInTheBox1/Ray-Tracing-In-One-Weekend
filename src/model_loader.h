#include "hittable_list.h"
#include "triangle.h"
#include "quaternion.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

void load_file(const char *filename, point3 position, vec3 scale, quaternion rotation, hittable_list &world, shared_ptr<material> mat);
void vertices_from_txt(const char *filename, std::vector<vec3> &vertices, std::vector<vec3> &tex_coords, point3 position, vec3 scale, quaternion rotation);
std::vector<std::string> split_string(std::string s, char c);