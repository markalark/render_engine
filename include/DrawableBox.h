/*
 * File:   DrawableBox.h
 * Author: Markalark
 *
 */

#ifndef DRAWABLEBOX_H
#define	DRAWABLEBOX_H
#include "Drawable.h"
#include "Transformable.h"
#include "Box.h"

//
class DrawableBox : public gl::Drawable, public Box {
public:
    DrawableBox(Vec3f position, Vec3f size) : Box(position, size) { }
};

#endif	/* DRAWABLEBOX_H */

