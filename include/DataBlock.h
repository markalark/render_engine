#ifndef DATABLOCK_H
#define	DATABLOCK_H
#include <functional>
#include "gl_headers.h"
#include <cstring>


namespace gl {
    /** a function for fetching and copying data into another location.
     * it takes the start address and size of the copy location and appends data to it*/
    typedef std::function<void(void *, size_t) > DataCopier;

    // specifies program data that is used to bind/copy into an opengl buffer
    // data is represented by a datacopier that fetches and copies memory to a specified address
    class DataBlock {
    public:
        DataBlock(DataCopier data_copier, size_t size) : data_copier(data_copier), size(size) { }
        DataBlock(void *start_address, size_t size) : size(size) {
            data_copier = [start_address](void *buffer_start, size_t buffer_size) {
                std::memcpy(buffer_start, start_address, buffer_size);
            };
        }

        // copy data block contents onto a given start address
        void copy_into(void *copy_start, size_t size) const {
            data_copier(copy_start, size);
        }

        // gettors
        GLsizei get_size() const {
            return size;
        }
    private:
        DataCopier data_copier;
        GLsizei size;
    };
};

#endif	/* DATABLOCK_H */

