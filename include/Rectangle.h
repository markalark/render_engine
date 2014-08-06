#ifndef RECTANGLE_H
#define	RECTANGLE_H

#include "Vec2.h"

//
class Rectangle {
public:
    Rectangle(Vec2f position, Vec2f size) : position(position), size(size) { }
    bool contains(Vec2f point) const {
        return (point.x >= position.x && point.y >= position.y &&
                point.x <= position.x + size.x && point.y <= position.y + size.y);
    }
    void set_position(Vec2f position) {
        this->position = position;
    }
    void set_size(Vec2f size) {
        this->size = size;
    }
    Vec2f get_position() const {
        return position;
    }
    Vec2f get_size() const {
        return size;
    }
private:
    Vec2f position;
    Vec2f size;
};

#endif	/* RECTANGLE_H */

