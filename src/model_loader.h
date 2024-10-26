#include "hittable_list.h"
#include "triangle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void load_file(const char *filename, point3 position, vec3 scale, hittable_list &world, shared_ptr<material> mat);
std::vector<std::string> split_string(std::string s, char c);