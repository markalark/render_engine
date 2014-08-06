#ifndef RENDERER_H
#define	RENDERER_H
#include <vector>
#include <queue>
#include <functional>
#include "Drawable.h"
#include "Storable.h"
#include "Attribute.h"
#include "GLUniform.h"
class Shader;
class Texture;
class Buffer;

// buffer and draw types are used to determine buffer placement for binds
enum BufferType {
    STREAM = 0,
    STATIC
};
enum DrawType {
    ARRAY = 0,
    ELEMENT
};

//
class Renderer {
public:
    Renderer(Vec2i screen_size);
    gl::Binding *bind(gl::Storable &storable, BufferType buffer_type, DrawType draw_type = ARRAY);
    // draw elements
    void draw(const gl::Drawable &draw_object,
              const gl::Storable &indices,
              const Shader *shader,
              const std::vector<gl::Uniform> &uniforms);
    // draw array
    void draw(const gl::Drawable &draw_object,
              const GLint offset,
              const GLsizei size,
              const Shader *shader,
              const std::vector<gl::Uniform> &uniforms);

    // set the number of output color targets for the framebuffer and whether depth buffer should be rendered
    void attach_render_targets(int color_textures);
    // get the textures that have been written to since the last call to 'set_frames'
    std::vector<Texture *> clear_render_targets();
    // add textures to the array of unused targets for future use
    void recycle_render_targets(const std::vector<Texture *> &targets);

    // for enabling / disabling properties externally
    void enable(GLenum property) {
        glEnable(property);
    }
    void disable(GLenum property) {
        glDisable(property);
    }

private:
    Buffer *find_available_buffer(const gl::DataBlock &data_block, BufferType buffer_type, DrawType draw_type);
    void bind_fbo_textures(int texture_count);
    void get_gl_buffer_properties(BufferType buffer_type, DrawType draw_type, GLenum &gl_buffer_type, GLenum &gl_draw_type);
    void enable_attributes(const std::vector<gl::Attribute> &attributes);
    void disable_attributes(const std::vector<gl::Attribute> &attributes);
    void set_uniforms(const Shader *shader, const std::vector<gl::Uniform> &uniform);

    // uniform settor functions (indexed by UniformType)
    static const std::function<void(GLint, GLsizei, GLboolean, void *) > set_uniform[];
    // all buffers (indexed like: buffers[buffer_type][draw_type])
    std::vector<std::vector<std::vector<Buffer *> > > buffers;

    // array of the fbo color attachments
    GLuint fbo_id;
    std::vector<GLenum> fbo_buffers;
    std::queue<Texture *> fbo_unused_textures;
    std::queue<Texture *> fbo_rendered_textures;

    // all bindings stored in buffers
    std::vector<gl::Binding *> bindings;
    GLuint vao_id;
};

#endif	/* RENDERER_H */

