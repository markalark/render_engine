/*
 * File:   DrawableRectangle.h
 * Author: Markalark
 *
 * Created on July 20, 2014, 2:32 AM
 */

#ifndef DRAWABLERECTANGLE_H
#define	DRAWABLERECTANGLE_H
#include "Rectangle.h"
#include "Drawable.h"

//
class DrawableRectangle : public gl::Drawable, public Rectangle {
public:
    DrawableRectangle(Vec2f position, Vec2f size) : Rectangle(position, size) { }
};

#endif	/* DRAWABLERECTANGLE_H */

