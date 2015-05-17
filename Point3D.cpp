#include "Point3D.h"
#include <cmath>

Point3D::Point3D() {
    set(0.0, 0.0, 0.0);
}

Point3D::Point3D(float x, float y, float z) {
    set(x, y, z);
}

Point3D::Point3D(const Point3D &other) {
    set(other.x, other.y, other.z);
}

Point3D::~Point3D() {
}

void Point3D::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point3D::set(const Point3D &other) {
    set(other.x, other.y, other.z);
}

float Point3D::getX() {
    return x;
}

float Point3D::getY() {
    return y;
}

float Point3D::getZ() {
    return z;
}

void Point3D::rotate(float ux, float uy, float uz, float degreeAngle) {
    /* Convert the degreeAngle from degree to radian */
    float radianAngle = degreeAngle * M_PI / 180;

    /* Calculate sine and cosine value of radianAngle */
    float c = cos(radianAngle);
    float s = sin(radianAngle);

    /* Calculate the coordinates of the point after rotating around vector u */
    float result_x = (c + (1 - c) * ux * ux) * x + ((1 - c) * uy * ux - s * uz) * y + ((1 - c) * uz * ux + s * uy) * z;
    float result_y = ((1 - c) * ux * uy + s * uz) * x + (c + (1 - c) * uy * uy) * y + ((1 - c) * uz * uy - s * ux) * z;
    float result_z = ((1 - c) * ux * uz - s * uy) * x + ((1 - c) * uy * uz + s * ux) * y + (c + (1 - c) * uz * uz) * z;

    /* Apply new coordinates to the current point */
    set(result_x, result_y, result_z);
}
