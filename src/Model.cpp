#include "Model.h"
#include "Attribute.h"

//
Model::Model(unsigned int id, std::vector<Vec3f> vertices, std::vector<Vec3f> normals, std::vector<Vec2f> uvs, std::vector<Vec3f> colors) :
    id(id), vertices(vertices), normals(normals), uvs(uvs), colors(colors) { }