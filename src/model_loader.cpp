#include "model_loader.h"
using namespace std;

void load_file(const char *filename, vector<vec3> &vertices, vector<vec3> &tex_coords)
{
    std::filesystem::path filePath = filename;

    if (filePath.extension().compare(".txt") == 0)
    {
        vertices_from_txt(filename, vertices, tex_coords);
    }
    else if (filePath.extension().compare(".obj") == 0)
    {
        vertices_from_obj(filename, vertices, tex_coords);
    }
}

// Loads vertices from a .txt file
void vertices_from_txt(const char *filename, std::vector<vec3> &vertices, std::vector<vec3> &tex_coords)
{
    ifstream file(filename);
    if (!file.good())
    {
        std::cout << "bad file " << filename;
    }
    string fileOut;
    while (getline(file, fileOut))
    {
        vector<string> coordList = split_string(fileOut, ' ');

        // Load and transform vertex
        vec3 vertex = vec3(atof(coordList[0].data()), atof(coordList[2].data()), -atof(coordList[1].data()));

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
void vertices_from_obj(const char *filename, std::vector<vec3> &vertices_out, std::vector<vec3> &tex_coords)
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

    file.close();
}

// Splits a string s based on character c
vector<string> split_string(string s, char c)
{
    stringstream stream(s);
    string segment;
    vector<string> seglist;

    while (getline(stream, segment, c))
    {
        // cout << segment << ", ";
        seglist.push_back(segment);
    }
    // cout << "\n";

    return seglist;
}