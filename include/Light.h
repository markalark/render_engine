/*
 * File:   Light.h
 * Author: Markalark
 *
 */

#ifndef LIGHT_H
#define	LIGHT_H

#include "Vec3.h"
#include "DrawableBox.h"

//
class Light : public DrawableBox {
public:
    Light(Vec3f position, Vec3f size, float intensity) :
    DrawableBox(position, size),
    intensity(intensity),
    color(1, 1, 1) { }

    //
    Vec3f get_color() {
        return color;
    }
    float get_intensity() {
        return intensity;
    }
private:
    float intensity;
    Vec3f color;
};

#endif	/* LIGHT_H */

