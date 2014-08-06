#include "Renderer.h"
#include "Binding.h"
#include "Shader.h"
#include "Texture.h"
#include "Buffer.h"


//
Renderer::Renderer(Vec2i screen_size) {
    //  make universal vao
    glGenVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);

    //  make universal fbo
    glGenFramebuffers(1, &this->fbo_id);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize the fbo attachments and textures
    for (int i = 0; i < 6; i++) {
        Texture *texture = new Texture(screen_size, false);
        fbo_unused_textures.push(texture);
        fbo_buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    // initialize buffer array (indexed by the buffer and draw types: buffers[buffer_type][draw_type])
    for (int i = 0; i < 2; i++) {
        buffers.push_back(std::vector<std::vector<Buffer *> >());
        for (int j = 0; j < 2; j++) {
            buffers[i].push_back(std::vector<Buffer *>());
        }
    }
}

// find a buffer that can hold the model data and has the least opengl state changes required for rendering
Buffer *Renderer::find_available_buffer(const gl::DataBlock &data_block, BufferType buffer_type, DrawType draw_type) {
    // determine amount of space needed for all model vertices
    size_t storage_needed = data_block.get_size();
    Buffer *available_buffer = nullptr;

    // find the buffer that can hold the space
    int i = 0;
    while (available_buffer == nullptr && i < buffers[buffer_type][draw_type].size()) {
        Buffer *buffer = buffers[buffer_type][draw_type][i];
        if (buffer->get_size() + storage_needed < buffer->get_max_size()) {
            available_buffer = buffer;
        }
        i++;
    }

    // if it couldn't find one...
    if (available_buffer == nullptr) {
        // make one
        GLenum gl_buffer_type, gl_draw_type;
        get_gl_buffer_properties(buffer_type, draw_type, gl_buffer_type, gl_draw_type);
        this->buffers[buffer_type][draw_type].push_back(new Buffer(gl_buffer_type, gl_draw_type));
        available_buffer = this->buffers[buffer_type][draw_type].back();
    }
    return available_buffer;
}

// bind a block of data with an opengl buffer, returning the buffer data
gl::Binding *Renderer::bind(gl::Storable &storable, BufferType buffer_type, DrawType draw_type) {
    // find buffer that can hold the space
    Buffer *buffer = find_available_buffer(storable.get_gl_data(), buffer_type, draw_type);
    GLvoid *buffer_offset = (GLvoid *) buffer->cursor;

    // append the data to the chosen buffer
    buffer->append(storable.get_gl_data());

    // add the binding and return it
    gl::Binding *binding = new gl::Binding(storable.get_gl_data(), buffer, 0, buffer_offset);
    storable.set_gl_binding(binding);
    bindings.push_back(binding);
    return binding;
}

//
void Renderer::draw(const gl::Drawable &draw_object,
                    const GLint offset,
                    const GLsizei size,
                    const Shader *shader,
                    const std::vector<gl::Uniform> &uniforms) {
    // enable the attributes (vertices, normals, etc)
    this->enable_attributes(draw_object.get_gl_attributes());

    // set the batch shader
    glUseProgram(shader->get_id());

    // set the uniforms
    set_uniforms(shader, uniforms);

    // draw
    glDrawArrays(GL_TRIANGLES, offset, size);

    // clear the state
    this->disable_attributes(draw_object.get_gl_attributes());
}

//
void Renderer::draw(const gl::Drawable &draw_object,
                    const gl::Storable &indices,
                    const Shader *shader,
                    const std::vector<gl::Uniform> &uniforms) {
    // enable the attributes (vertices, normals, etc)
    this->enable_attributes(draw_object.get_gl_attributes());

    // bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.get_gl_binding()->get_buffer()->get_vbo_id());

    // set the batch shader
    glUseProgram(shader->get_id());

    // set the uniforms
    set_uniforms(shader, uniforms);

    // draw
    GLsizei element_count = indices.get_gl_data().get_size() / (sizeof (unsigned int));
    glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, indices.get_gl_binding()->get_offset());

    // clear the state
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    this->disable_attributes(draw_object.get_gl_attributes());
}

// bind an amount of unused fbo textures onto the current fbo's attachments
void Renderer::bind_fbo_textures(int texture_count) {
    for (int i = 0; i < texture_count; i++) {
        // pop a texture
        Texture *texture = fbo_unused_textures.front();
        fbo_unused_textures.pop();
        // attach the texture to the fbo attachment (fragment output target)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D, texture->get_id(), 0);

        // add the texture the list of current render targets (so they can be returned after drawing)
        fbo_rendered_textures.push(texture);
    }
    // tell opengl to use these attachments for draws
    glDrawBuffers(texture_count, fbo_buffers.data());
}

//
void Renderer::attach_render_targets(int target_count) {
    // the initial bind
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

    // bind the needed textures to the fbo color attachments
    bind_fbo_textures(target_count);

    // reset buffer
    glEnable(GL_BLEND);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // change the blend type
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);
}

//
std::vector<Texture *> Renderer::clear_render_targets() {
    // set gl settings to default
    for (int i = 0; i < 5; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D, 0, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.2f, 0.2f, 1);
    glUseProgram(0);

    // copy of the rendered textures for return
    std::vector<Texture *> rendered_targets;

    // clear the array of currently used render targets
    while (!fbo_rendered_textures.empty()) {
        rendered_targets.push_back(fbo_rendered_textures.front());
        fbo_rendered_textures.pop();
    }

    // return the render targets
    return rendered_targets;
}

// add textures to the array of unused targets for future use
void Renderer::recycle_render_targets(const std::vector<Texture *> &targets) {
    for (auto &target : targets) {
        if (target->get_format() == GL_RGBA8) {
            fbo_unused_textures.push(target);
        }
    }
}

//
void Renderer::get_gl_buffer_properties(BufferType buffer_type, DrawType draw_type,
                                        GLenum &gl_buffer_type, GLenum &gl_draw_type) {
    // defaults
    gl_buffer_type = GL_ARRAY_BUFFER;
    gl_draw_type = GL_STATIC_DRAW;

    // special cases
    if (draw_type == ELEMENT) {
        gl_buffer_type = GL_ELEMENT_ARRAY_BUFFER;
    }
    if (buffer_type == STREAM) {
        gl_draw_type = GL_STREAM_DRAW;
    }
}

//
void Renderer::disable_attributes(const std::vector<gl::Attribute> &attributes) {
    for (auto &attribute : attributes) {
        // disable it
        glDisableVertexAttribArray(attribute.location);
    }
    // disable array buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//
void Renderer::enable_attributes(const std::vector<gl::Attribute> &attributes) {
    // set the meta for each attribute (vertex, normal, uv, bones, bone weights)
    for (auto &attribute : attributes) {
        // bind the buffer used by the attribute
        glBindBuffer(GL_ARRAY_BUFFER, attribute.get_gl_binding()->get_buffer()->get_vbo_id());

        // set attribute
        glVertexAttribPointer(attribute.location, // location mapped in shader file
                              attribute.data_count, // size
                              attribute.data_type, // type (GL_FLOAT, etd)
                              attribute.normalized, // normalized?
                              attribute.get_gl_binding()->get_stride(), // stride
                              attribute.get_gl_binding()->get_offset()); // starting offset

        // enable it
        glEnableVertexAttribArray(attribute.location);
    }
}
void Renderer::set_uniforms(const Shader *shader, const std::vector<gl::Uniform> &uniforms) {
    int texture_counter = 0;
    for (auto &uniform : uniforms) {
        if (uniform.data_type != gl::Uniform::TEXTURE) {
            set_uniform[uniform.data_type](shader->get_uniform_location(uniform.id), uniform.data_count, uniform.transposed, uniform.data);
        } else {
            Texture *texture = (Texture *) uniform.data;
            glActiveTexture(GL_TEXTURE0 + texture_counter);
            glBindTexture(GL_TEXTURE_2D, texture->get_id());
            glUniform1i(shader->get_uniform_location(uniform.id), texture_counter);
            texture_counter++;
        }
    }
}

// functions for setting up different uniform types (indexed by the uniform type)
const std::function<void(GLint, GLsizei, GLboolean, void *) > Renderer::set_uniform[] = {
    // MATRIX4f
    [](GLint location, GLsizei data_count, GLboolean transposed, void *data) {
        glUniformMatrix4fv(location, data_count, transposed, (GLfloat *) data);
    },
    // FLOAT_ARRAY
    [](GLint location, GLsizei data_count, GLboolean transposed, void *data) {
        glUniform1fv(location, data_count, (GLfloat *) data);
    },
    // INT_ARRAY
    [](GLint location, GLsizei data_count, GLboolean transposed, void *data) {
        glUniform1iv(location, data_count, (GLint *) data);
    },
    // VEC2i
    [](GLint location, GLsizei data_count, GLboolean transposed, void *data) {
        glUniform2iv(location, data_count, (GLint *) data);
    },
    // VEC3i
    [](GLint location, GLsizei data_count, GLboolean transposed, void *data) {
        glUniform3iv(location, data_count, (GLint *) data);
    },
    // VEC2f
    [](GLint location, GLsizei data_count, GLboolean transposed, void *data) {
        glUniform2fv(location, data_count, (GLfloat *) data);
    },
    // VEC3f
    [](GLint location, GLsizei data_count, GLboolean transposed, void *data) {
        glUniform3fv(location, data_count, (GLfloat *) data);
    }
};
