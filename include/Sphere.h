/*
 * File:   Sphere.h
 * Author: Markalark
 *
 * Created on July 20, 2014, 3:36 PM
 */

#ifndef SPHERE_H
#define	SPHERE_H
#include "Vec3.h"
#include <cmath>

//
class Sphere {
    Sphere(Vec3f position, float radius) : position(position), radius(radius) { }
    bool contains(Vec3f position) {
        Vec3f difference = this->position - position;
        float distance = difference.dot(difference);
        return distance < std::pow(radius, 2);
    }
private:
    Vec3f position;
    float radius;
};

#endif	/* CIRCLE_H */

