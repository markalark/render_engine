/*
 * File:   Storable.h
 * Author: Markalark
 *
 */

#ifndef STORABLE_H
#define	STORABLE_H
#include "DataBlock.h"
#include "Binding.h"

namespace gl {
    class Storable {
    public:
        Storable() : data_block(NULL, 0), binding(nullptr) { }
        Storable(DataBlock data_block) : data_block(data_block), binding(nullptr) { }
        void set_gl_binding(gl::Binding *binding) {
            this->binding = binding;
        }
        gl::Binding *get_gl_binding() const {
            return binding;
        }
        void set_gl_data(const DataBlock &data_block) {
            this->data_block = data_block;
        }
        const gl::DataBlock &get_gl_data() const {
            return data_block;
        }
    private:
        DataBlock data_block;
        gl::Binding *binding;
    };
}

#endif	/* STORABLE_H */

