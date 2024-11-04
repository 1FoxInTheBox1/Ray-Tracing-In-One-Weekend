#include "model_loader.h"
using namespace std;

void load_file(const char *filename, point3 position, vec3 scale, quaternion rotation, hittable_list &world, shared_ptr<material> mat)
{
    vector<vec3> vertices;
    vector<vec3> tex_coords;
    std::filesystem::path filePath = filename;

    if (filePath.extension().compare(".txt") == 0)
    {
        vertices_from_txt(filename, vertices, tex_coords, position, scale, rotation);
    }

    for (int i = 0; i < vertices.size(); i += 3) 
    {
        shared_ptr<triangle> tri;
        if (!tex_coords.empty()) 
        {
            tri = make_shared<triangle>(vertices[i], vertices[i + 1], vertices[i + 2], 
                                                tex_coords[i], tex_coords[i + 1], tex_coords[i + 2], mat);
        } 
        else
        {
            tri = make_shared<triangle>(vertices[i], vertices[i + 1], vertices[i + 2], mat);
        }
        // cout << "Added triangle " << *tri << "\n";
        world.add(tri);
    }

}

void vertices_from_txt(const char *filename, std::vector<vec3> &vertices, std::vector<vec3> &tex_coords, point3 position, vec3 scale, quaternion rotation)
{
    ifstream file(filename);
    string fileOut;
    while (getline(file, fileOut)) 
    {
        vector<string> coordList = split_string(fileOut, ' ');
        
        vec3 vertex = vec3(atof(coordList[0].data()), atof(coordList[2].data()), -atof(coordList[1].data()));
        vertex *= scale;
        vertex.rotate(rotation);
        vertex += position;
        // cout << "Added vertex " << vertex << "\n";

        if (coordList.size() >= 5) 
        {
            vec3 tex_coord = vec3(atof(coordList[3].data()), atof(coordList[4].data()), 0.0);
            tex_coords.push_back(tex_coord);
        } 

        vertices.push_back(vertex);
    }

    file.close();
}

// Split a string s based on character c
vector<string> split_string(string s, char c)
{
    stringstream stream(s);
    string segment;
    vector<string> seglist;

    while(getline(stream, segment, c))
    {
        // cout << segment << ", ";
        seglist.push_back(segment);
    }
    // cout << "\n";

    return seglist;
}