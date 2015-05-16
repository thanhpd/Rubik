#include <cmath>
#include "Vector3D.h"

Vector3D::Vector3D() {
    set(0.0, 0.0, 0.0);
}

Vector3D::Vector3D(float x, float y, float z) {
    set(x, y, z);
}

Vector3D::Vector3D(const Vector3D &other) {
    set(other);
}

Vector3D::~Vector3D() {

}

void Vector3D::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3D::set(const Vector3D &other) {
    set(other.x, other.y, other.z);
}

float Vector3D::getX() {
    return x;
}

float Vector3D::getY() {
    return y;
}

float Vector3D::getZ() {
    return z;
}

float Vector3D::length() {
    return (float) sqrt(x * x + y * y + z * z);
}

void Vector3D::setDiff(Point3D A, Point3D B) {
    x = B.getX() - A.getX();
    y = B.getY() - A.getY();
    z = B.getZ() - A.getZ();
}

void Vector3D::normalize() {
    float len = length();

    set(x / len, y / len, z / len);
}

float Vector3D::dot(Vector3D other) {
    return x * other.getX() + y * other.getY() + z * other.getZ();
}

Vector3D Vector3D::cross(Vector3D other) {
    float tempX = y * other.getZ() - z * other.getY();
    float tempY = z * other.getX() - x * other.getZ();
    float tempZ = x * other.getY() - y * other.getX();

    return Vector3D(tempX, tempY, tempZ);
}


