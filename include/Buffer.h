#ifndef BUFFER_H
#define	BUFFER_H
#include <string>
#include <cstring>
#include "Storable.h"

//
class Buffer {
public:
    Buffer(size_t max_size, GLenum buffer_type, GLenum draw_type) : max_size(max_size),
    size(0), cursor(0), buffer_type(buffer_type), draw_type(draw_type) {
        glGenBuffers(1, &this->vbo_id);
        glBindBuffer(buffer_type, this->vbo_id);
        glBufferData(buffer_type,
                     // size of buffer to initialize
                     max_size,
                     // no memory to copy
                     NULL,
                     // buffer draw type (stream / static)
                     draw_type);
        // clear buffer bind
        glBindBuffer(buffer_type, 0);
    }
    Buffer(GLenum buffer_type, GLenum draw_type) : Buffer(3000000, buffer_type, draw_type) { }

    // append data from a DataCopier onto the opengl buffer
    void append(const gl::DataBlock &data_block) {
        glBindBuffer(buffer_type, this->vbo_id);
        void *buffer_offset = glMapBufferRange(buffer_type,
                                               this->cursor,
                                               data_block.get_size(),
                                               GL_MAP_WRITE_BIT
                                               | GL_MAP_UNSYNCHRONIZED_BIT);
        data_block.copy_into(buffer_offset, data_block.get_size());
        this->cursor += (GLintptr) data_block.get_size();
        glUnmapBuffer(buffer_type);
    }

    // append the data to the opengl buffer
    void append(GLvoid *start_address, GLsizei size) {
        glBindBuffer(buffer_type, this->vbo_id);
        void *buffer_offset = glMapBufferRange(buffer_type,
                                               this->cursor,
                                               size,
                                               GL_MAP_WRITE_BIT
                                               | GL_MAP_UNSYNCHRONIZED_BIT);
        std::memcpy(buffer_offset, start_address, size);
        this->cursor += (GLintptr) size;
        glUnmapBuffer(buffer_type);
    }
    GLuint get_vbo_id() const {
        return vbo_id;
    }
    size_t get_max_size() const {
        return max_size;
    }
    size_t get_size() const {
        return size;
    }
    GLintptr cursor;
private:
    size_t max_size;
    size_t size;
    GLuint vbo_id;
    GLenum buffer_type;
    GLenum draw_type;
};

#endif	/* BUFFER_H */

