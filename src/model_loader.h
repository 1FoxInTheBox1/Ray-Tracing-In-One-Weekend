#include "hittable_list.h"
#include "triangle.h"
#include "quaternion.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

void load_file(const char *filename, std::vector<vec3> &vertices, std::vector<vec3> &tex_coords);
void vertices_from_txt(const char *filename, std::vector<vec3> &vertices, std::vector<vec3> &tex_coords);
void vertices_from_obj(const char *filename, std::vector<vec3> &vertices_out, std::vector<vec3> &tex_coords);
std::vector<std::string> split_string(std::string s, char c);