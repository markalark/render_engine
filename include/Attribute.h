#ifndef ATTRIBUTE_H
#define	ATTRIBUTE_H

#include "Storable.h"
#include "DataBlock.h"

// attribute
namespace gl {
    class Attribute : public gl::Storable {
    public:
        Attribute(int location, int data_count, GLenum data_type, bool normalized, DataBlock data_block) :
        gl::Storable(data_block),
        location(location),
        data_type(data_type),
        data_count(data_count),
        normalized(normalized) { }

        // data
        int location;
        GLenum data_type;
        int data_count;
        bool normalized;
    };
};

#endif	/* ATTRIBUTE_H */

