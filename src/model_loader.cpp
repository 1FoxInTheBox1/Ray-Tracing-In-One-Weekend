#include "model_loader.h"
using namespace std;

void load_file(const char *filename, point3 position, hittable_list &world, shared_ptr<material> mat)
{
    string fileOut;
    vector<vec3> vertices;
    vector<vec3> tex_coords;

    ifstream file(filename);

    int i = 0;
    while (getline(file, fileOut)) 
    {
        vector<string> coordList = split_string(fileOut, ' ');
        
        vec3 vertex = vec3(atof(coordList[0].data()), atof(coordList[1].data()), atof(coordList[2].data()));
        vertex += position;
        // cout << "Added vertex " << vertex << "\n";

        if (coordList.size() >= 5) 
        {
            vec3 tex_coord = vec3(atof(coordList[3].data()), atof(coordList[4].data()), 0.0);
            tex_coords.push_back(tex_coord);
        } 

        vertices.push_back(vertex);
    }

    for (i = 0; i < vertices.size(); i += 3) 
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