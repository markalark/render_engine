#include "gl_headers.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//
Texture::Texture(Vec2i size, bool depth_texture) : resolution(size) {
    this->id = create_render_texture(size, depth_texture);
}

//
Texture::Texture(std::string filepath) {
    glGenTextures(1, &this->id);
    int force_channels = 4;
    unsigned char *data = load_image_data(filepath, force_channels, resolution);
    bind_texture_data(this->id, data, resolution);
}

//
unsigned char *Texture::load_image_data(std::string filepath, int channel_count, Vec2i &resolution) {
    // number of channels per pixel implied by the file (not always the number we want)
    int n;

    // get the data from the image file (forcing the number of channels per pixel with 'channel_count')
    unsigned char *pixel_data = stbi_load(filepath.c_str(), &resolution.x, &resolution.y, &n, channel_count);
    if (!pixel_data) {
        std::cout << "texture file: " << filepath << " couldn't be read." << std::endl;
    } else {
        std::cout << "channels: " << n << std::endl;
    }

    // check if dimensions aren't a power of 2
    if ((resolution.x & (resolution.x - 1)) != 0 || (resolution.y & (resolution.y - 1)) != 0) {
        std::cout << filepath << " has a texture size that is not a power of 2" << std::endl;
    }
    return pixel_data;
}

//
void Texture::bind_texture_data(GLuint id, unsigned char *data, Vec2i resolution) {
    this->format = GL_RGBA8;
    // store the image data into opengl
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D,
                 0, // base level (no minimap)
                 GL_RGBA8, // internal format
                 resolution.x, // width
                 resolution.y, // height
                 0, // border
                 GL_RGBA, // format
                 GL_UNSIGNED_BYTE, // data type
                 data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    check_error();
}

// create a texture that can be rendered to. indicate whether it's a color or depth texture
GLint Texture::create_render_texture(Vec2i size, bool depth) {
    GLuint text_id;
    glGenTextures(1, &text_id);
    this->format = depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8;
    glBindTexture(GL_TEXTURE_2D, text_id);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 depth ? GL_DEPTH_COMPONENT24 : GL_RGB32F,
                 size.x, size.y, 0,
                 depth ? GL_DEPTH_COMPONENT : GL_RGB,
                 GL_FLOAT, NULL);

    //unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    return text_id;
}

//
GLuint Texture::check_error() {
    GLuint err = glGetError();
    if (err > 0) {
        std::cout << "Texture error: 0x" << std::hex << err << std::endl;
    }
    return err;
}
