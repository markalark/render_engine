#ifndef TEXTURE_H
#define	TEXTURE_H

#include "gl_headers.h"
#include "Vec2.h"
#include <string>

//
class Texture {
public:
    Texture(Vec2i size, bool depth_texture);
    Texture(std::string texture_image);
    GLuint get_id() const {
        return id;
    }
    Vec2i get_resolution() const {
        return resolution;
    }
    GLenum get_format() const {
        return format;
    }
private:
    GLuint id;
    Vec2i resolution;
    GLenum format;
    void bind_texture_data(GLuint id, unsigned char *data, Vec2i resolution);
    unsigned char *load_image_data(std::string filepath, int channel_count, Vec2i &resolution);
    GLint create_render_texture(Vec2i size, bool depth);
    GLuint check_error();
};

#endif	/* TEXTURE_H */

