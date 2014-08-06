#ifndef BINDING_H
#define	BINDING_H

#include "gl_headers.h"
#include "DataBlock.h"
#include "Buffer.h"

// the bind information of a data block which has been copied into an opengl buffer
namespace gl {
    class Binding {
    public:
        Binding(DataBlock data_block, Buffer *buffer, GLsizei stride, GLvoid *offset) :
        data_block(data_block), buffer(buffer), stride(stride), offset(offset) { }
        DataBlock get_data_block() const {
            return data_block;
        }
        const Buffer *get_buffer() const {
            return buffer;
        }
        GLvoid *get_offset() const {
            return offset;
        }
        GLsizei get_stride() const {
            return stride;
        }
    private:
        DataBlock data_block;
        Buffer *buffer;
        GLvoid *offset;
        GLsizei stride;

    };
};

#endif	/* BINDING_H */

