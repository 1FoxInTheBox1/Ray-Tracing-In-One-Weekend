#include "model_loader.h"
using namespace std;

void load_file(const char *filename, hittable_list &world, shared_ptr<material> mat)
{
    string fileOut;
    vector<vec3> vertices;

    ifstream file(filename);

    int i = 0;
    while (getline(file, fileOut)) {
        vector<string> coordList = split_string(fileOut, ' ');
        vec3 vertex = vec3(atof(coordList[0].data()), atof(coordList[1].data()), atof(coordList[2].data()));
        cout << "Added vertex " << vertex << "\n";
        vertices.push_back(vertex);
    }

    for (i = 0; i < vertices.size(); i++) {
        world.add(make_shared<triangle>(vertices[i], vertices[i + 1], vertices[i + 2], mat));
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
        cout << segment << ", ";
        seglist.push_back(segment);
    }
    cout << "\n";

    return seglist;
}