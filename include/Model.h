#ifndef MODEL_H
#define	MODEL_H

#include "gl_headers.h"
#include "Vec3.h"
#include "Vec2.h"
#include "Drawable.h"
#include "Transformable.h"
#include "VertexGroup.h"
#include "Attribute.h"

//
class Model : public gl::Drawable, public Transformable {
public:
    Model(unsigned int id,
          std::vector<Vec3f> vertices,
          std::vector<Vec3f> normals,
          std::vector<Vec2f> uvs,
          std::vector<Vec3f> colors = {});
    unsigned int get_id() {
        return id;
    }
    void add_vertex_group(const std::vector<unsigned int> &indices, Texture *texture, Shader *shader) {
        vertex_groups.push_back(VertexGroup(indices, texture, shader));
    }
    std::vector<VertexGroup> &get_vertex_groups() {
        return vertex_groups;
    }
    const std::vector<Vec3f> &get_vertices() const {
        return vertices;
    }
    const std::vector<Vec3f> &get_normals() const {
        return normals;
    }
    const std::vector<Vec2f> &get_uvs() const {
        return uvs;
    }
private:
    std::vector<VertexGroup> vertex_groups;

    // mesh properties
    unsigned int id;
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> uvs;
    std::vector<Vec3f> colors;
};

#endif	/* MODEL_H */