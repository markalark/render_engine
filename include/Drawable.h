/*
 * File:   Drawable.h
 * Author: Markalark
 *
 */

#ifndef DRAWABLE_H
#define	DRAWABLE_H
#include <vector>
#include "Attribute.h"

namespace gl {
    class Drawable {
    public:
        void add_gl_attribute(gl::Attribute attribute) {
            attributes.push_back(attribute);
        }
        std::vector<gl::Attribute> &get_gl_attributes() {
            return attributes;
        }
        const std::vector<gl::Attribute> &get_gl_attributes() const {
            return attributes;
        }
    private:
        std::vector<gl::Attribute> attributes;
    };
}

#endif	/* DRAWABLE_H */

