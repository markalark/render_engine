/*
 * File:   Box.h
 * Author: Markalark
 *
 */

#ifndef BOX_H
#define	BOX_H
#include "Vec3.h"

//
class Box : public Transformable {
public:
    Box(Vec3f position, Vec3f size) : Transformable(position), size(size) { }

    //
    bool contains(Vec3f point) const {
        Vec3f position = this->get_position();
        return (point.x >= position.x &&
                point.y >= position.y &&
                point.z >= position.z &&
                point.x <= position.x + size.x &&
                point.y <= position.y + size.y &&
                point.z <= position.z + size.z);
    }
    void set_size(Vec3f size) {
        this->size = size;
    }
    Vec3f get_size() {
        return size;
    }
    std::vector<Vec3f> get_vertices() const {
        std::vector<Vec3f> vertices = {
            // front
            Vec3f(-size.x / 2, -size.y / 2, size.z / 2),
            Vec3f(size.x / 2, -size.y / 2, size.z / 2),
            Vec3f(size.x / 2, size.y / 2, size.z / 2),
            Vec3f(size.x / 2, size.y / 2, size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, size.z / 2),
            Vec3f(-size.x / 2, -size.y / 2, size.z / 2),
            // top
            Vec3f(-size.x / 2, size.y / 2, size.z / 2),
            Vec3f(size.x / 2, size.y / 2, size.z / 2),
            Vec3f(size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, size.z / 2),
            // back
            Vec3f(size.x / 2, -size.y / 2, -size.z / 2),
            Vec3f(-size.x / 2, -size.y / 2, -size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, -size.y / 2, -size.z / 2),
            // bottom
            Vec3f(-size.x / 2, -size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, -size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, -size.y / 2, size.z / 2),
            Vec3f(size.x / 2, -size.y / 2, size.z / 2),
            Vec3f(-size.x / 2, -size.y / 2, size.z / 2),
            Vec3f(-size.x / 2, -size.y / 2, -size.z / 2),
            // left
            Vec3f(-size.x / 2, -size.y / 2, -size.z / 2),
            Vec3f(-size.x / 2, -size.y / 2, size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, size.z / 2),
            Vec3f(-size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(-size.x / 2, -size.y / 2, -size.z / 2),
            // right
            Vec3f(size.x / 2, -size.y / 2, size.z / 2),
            Vec3f(size.x / 2, -size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, size.y / 2, -size.z / 2),
            Vec3f(size.x / 2, size.y / 2, size.z / 2),
            Vec3f(size.x / 2, -size.y / 2, size.z / 2),
        };
        return vertices;
    }
private:
    Vec3f size;
};

#endif	/* CUBOID_H */

