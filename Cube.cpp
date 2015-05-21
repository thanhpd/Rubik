#include "Cube.h"

Cube::Cube() { }

Cube::Cube(Face f0, Face f1, Face f2, Face f3, Face f4, Face f5) {
	setFace(f0, f1, f2, f3, f4, f5);
}

void Cube::setFace(Face f0, Face f1, Face f2, Face f3, Face f4, Face f5) {
	face[0].set(f0); Point3D p0 = f0.getCenter();
	face[1].set(f1); Point3D p1 = f1.getCenter();
	face[2].set(f2); Point3D p2 = f2.getCenter();
	face[3].set(f3); Point3D p3 = f3.getCenter();
	face[4].set(f4); Point3D p4 = f4.getCenter();
	face[5].set(f5); Point3D p5 = f5.getCenter();

    float cenx = (p0.getX() + p1.getX() + p2.getX() + p3.getX() + p4.getX() + p5.getX()) / 6;
    float ceny = (p0.getY() + p1.getY() + p2.getY() + p3.getY() + p4.getY() + p5.getY()) / 6;
    float cenz = (p0.getZ() + p1.getZ() + p2.getZ() + p3.getZ() + p4.getZ() + p5.getZ()) / 6;
    center.set(cenx, ceny, cenz);
}

void Cube::set(Cube &cube) {
	setFace(cube.face[0], cube.face[1], cube.face[2], cube.face[3], cube.face[4], cube.face[5]);
	setIndex(cube.getX(), cube.getY(), cube.getZ());
}

void Cube::draw() {
	for (int i = 0; i < NUM_FACES; i++)
		face[i].draw();	
}

void Cube::rotate(Vector3D u, float angle) {
	for (int i = 0; i < NUM_FACES; i++)
		face[i].rotate(u, angle);
	center.rotate(u.getX(), u.getY(), u.getZ(), angle);
}

Point3D Cube::getCenter() {
	return center;
}

void Cube::setIndex(int x, int y, int z) {
	setX(x); setY(y); setZ(z);
}

void Cube::setX(int x) {
	this->x = x;
}

int Cube::getX() {
	return x;
}

void Cube::setY(int y) {
	this->y = y;
}

int Cube::getY() {
	return y;
}

void Cube::setZ(int z) {
	this->z = z;
}

int Cube::getZ() {
	return z;
}

