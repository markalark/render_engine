#ifndef GLUNIFORM_H
#define	GLUNIFORM_H

#include "gl_headers.h"
#include "Texture.h"

namespace gl {
    class Uniform {
    public:
        // possible datatypes for the uniform
        enum Type {
            MATRIX4f_ARRAY = 0,
            FLOAT_ARRAY,
            INT_ARRAY,
            VEC2i,
            VEC3i,
            VEC2f,
            VEC3f,
            TEXTURE
        };

        // constructors
        Uniform(int id, Uniform::Type type, GLsizei data_count, GLboolean transposed, void *data) :
        id(id), data_type(type), data_count(data_count), transposed(transposed), data(data) { }
        Uniform(int id, Uniform::Type type, GLsizei data_count, void *data) :
        id(id), data_type(type), data_count(data_count), transposed(false), data(data) { }

        // data
        Uniform::Type data_type;
        int id;
        GLsizei data_count;
        GLboolean transposed;
        void *data;
    };

    // shortcut functions for creating specific uniforms
    inline Uniform Uniform1i(int id, void *value) {
        return Uniform(id, Uniform::INT_ARRAY, 1, false, (void *) value);
    }
    inline Uniform Uniform1f(int id, void *value) {
        return Uniform(id, Uniform::FLOAT_ARRAY, 1, false, (void *) value);
    }
    inline Uniform UniformVec2i(int id, void *vec) {
        return Uniform(id, Uniform::VEC2i, 1, false, vec);
    }
    inline Uniform UniformVec3i(int id, void *vec) {
        return Uniform(id, Uniform::VEC3i, 1, false, vec);
    }
    inline Uniform UniformVec2f(int id, void *vec) {
        return Uniform(id, Uniform::VEC2f, 1, false, vec);
    }
    inline Uniform UniformVec3f(int id, void *vec) {
        return Uniform(id, Uniform::VEC3f, 1, false, vec);
    }
    inline Uniform UniformMatrix4f(int id, bool transposed, void *matrix) {
        return Uniform(id, Uniform::MATRIX4f_ARRAY, 1, false, matrix);
    }
    inline Uniform UniformTexture(int id, Texture *texture) {
        return Uniform(id, Uniform::TEXTURE, 1, false, (void *) texture);
    }
};

#endif	/* GLUNIFORM_H */

