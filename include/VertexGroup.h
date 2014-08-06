#ifndef VERTEXGROUP_H
#define	VERTEXGROUP_H

#include "Storable.h"
#include "Texture.h"
#include "Shader.h"

//
class VertexGroup : public gl::Storable {
public:
    VertexGroup(const std::vector<unsigned int> &indices, Texture *texture, Shader *shader) :
    vertex_indices(indices), texture(texture), shader(shader) {
        size_t data_size = sizeof (this->vertex_indices[0]) * this->vertex_indices.size();
        this->set_gl_data(gl::DataBlock(this->vertex_indices.data(), data_size));
    }

    // indices are ordered according to the vertex array in the model
    const std::vector<unsigned int> &get_indices() {
        return vertex_indices;
    }

    // get texture / shader
    Texture *get_texture() {
        return texture;
    }
    Shader *get_shader() {
        return shader;
    }

private:
    Texture *texture;
    Shader *shader;
    std::vector<unsigned int> vertex_indices;
};

#endif	/* VERTEXGROUP_H */

