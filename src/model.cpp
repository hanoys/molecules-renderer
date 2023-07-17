#include "model.h"

#include <iostream>
#include <qvector3d.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <QVector3D>

Model::Model(const char *filename) : verts(), faces() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()){
        std::cout << "fail";
        return;
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            float x, y, z;
            iss >> x >> y >> z;
            vec3 v{x, y , z};
            verts.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }
            faces.push_back(f);
        }
    }
    std::cout << "# v# " << verts.size() << " f# "  << faces.size() << std::endl;
}

void Model::rotate(mat4 m) {
    for (int i = 0; i < verts.size(); i++) {
        verts[i] = m * verts[i];
    }
}
