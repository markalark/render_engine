/*
 * File:   Transformable.h
 * Author: Markalark
 *
 */

#ifndef TRANSFORMABLE_H
#define	TRANSFORMABLE_H
#include "Vec3.h"
class Transformable {
public:
    Transformable(Vec3f position = Vec3f(0, 0, 0)) : position(position), rotation(1), scale(1) { }
    glm::mat4 get_transform() const {
        glm::mat4 translation = glm::translate(glm::vec3(position.x, position.y, position.z));
        return rotation * translation;
    }
    void rotate(float radians, Vec3f axis) {
        glm::mat4 rotation_matrix = glm::rotate(radians, glm::vec3(axis.x, axis.y, axis.z));
        rotation *= rotation_matrix;
    }
    void move(Vec3f movement) {
        position += movement;
    }
    Vec3f get_position() const {
        return position;
    }
    void set_position(Vec3f position) {
        this->position = position;
    }
private:
    glm::mat4 rotation;
    Vec3f position;
    float scale;
};

#endif	/* TRANSFORMABLE_H */

