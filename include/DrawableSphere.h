/*
 * File:   DrawableSphere.h
 * Author: Markalark
 *
 */

#ifndef DRAWABLESPHERE_H
#define	DRAWABLESPHERE_H
#include <vector>
#include "Vec3.h"
#include "Drawable.h"
#include "Sphere.h"

//
class DrawableSphere : public Drawable, public Sphere {
public:
    DrawableSphere(Vec3f position, float radius, int vertex_resolution) : Sphere(position, radius) {
        // TODO
    }
private:
    std::vector<Vec3f> vertices;
};

#endif	/* DRAWABLESPHERE_H */

