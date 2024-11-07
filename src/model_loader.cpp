#include "model_loader.h"
using namespace std;

// Loads triangles from the given file and places them into the hittable_list tri_list
// TODO: Load from .obj files
void load_file(const char *filename, point3 position, vec3 scale, quaternion rotation, hittable_list &tri_list, shared_ptr<material> mat)
{
    vector<vec3> vertices;
    vector<vec3> tex_coords;
    std::filesystem::path filePath = filename;

    if (filePath.extension().compare(".txt") == 0)
    {
        vertices_from_txt(filename, vertices, tex_coords, position, scale, rotation);
    } 
    else if (filePath.extension().compare(".obj") == 0)
    {
        vertices_from_obj(filename, vertices, tex_coords, position, scale, rotation);
    }

    make_triangles(vertices, tex_coords, mat, tri_list);

}

// Loads vertices from a .txt file
void vertices_from_txt(const char *filename, std::vector<vec3> &vertices, std::vector<vec3> &tex_coords, point3 position, vec3 scale, quaternion rotation)
{
    ifstream file(filename);
    string fileOut;
    while (getline(file, fileOut)) 
    {
        vector<string> coordList = split_string(fileOut, ' ');
        
        // Load and transform vertex
        vec3 vertex = vec3(atof(coordList[0].data()), atof(coordList[2].data()), -atof(coordList[1].data()));
        vertex *= scale;
        vertex.rotate(rotation);
        vertex += position;

        // If the file contains texture coordinates then load those as well
        if (coordList.size() >= 5) 
        {
            vec3 tex_coord = vec3(atof(coordList[3].data()), atof(coordList[4].data()), 0.0);
            tex_coords.push_back(tex_coord);
        } 

        vertices.push_back(vertex);
    }

    file.close();
}

// Loads vertices from a .obj file
void vertices_from_obj(const char *filename, std::vector<vec3> &vertices_out, std::vector<vec3> &tex_coords, point3 position, vec3 scale, quaternion rotation)
{
    ifstream file(filename);
    string fileOut;
    std::vector<vec3> vertex_list;
    while (getline(file, fileOut)) 
    {
        if (fileOut[0] != '#' && fileOut.length() != 0)
        {
            vector<string> split_line = split_string(fileOut, ' ');
            // Load in a vertex
            if (split_line[0].compare("v") == 0)
            {
                vec3 vertex = vec3(atof(split_line[1].data()), atof(split_line[3].data()), -atof(split_line[2].data()));
                vertex *= scale;
                vertex.rotate(rotation);
                vertex += position;
                vertex_list.push_back(vertex);
            }

            // Load in a face
            if (split_line[0].compare("f") == 0)
            {
                for (int i = 1; i < split_line.size(); i++)
                {
                    vector<string> vertex_data = split_string(split_line[i], '/');
                    vec3 vertex_pos = vertex_list[atoi(vertex_data[0].data()) - 1];
                    // std::cout << "vertex pos: " << vertex_pos << "\n";
                    vec3 vertex = vertex_pos;
                    vertices_out.push_back(vertex);
                }
            }
        }
    }

    // for (vec3 vert : vertices_out)
    // {
    //     std:cout << vert << "\n";
    // }

    file.close();
}

// Create triangles from the vertices list
// Note that texture coords are optional,
// and that we do not need to load normals 
// since we calculate those ourselves
void make_triangles(std::vector<vec3> &vertices, std::vector<vec3> &tex_coords, shared_ptr<material> mat, hittable_list &tri_list)
{
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
        tri_list.add(tri);
    }
}

// Splits a string s based on character c
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