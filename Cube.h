#ifndef RUBIK_CUBE_H
#define RUBIK_CUBE_H

#include "Point3D.h"
#include "Vector3D.h"
#include "Face.h"

#define NUM_FACES 6

class Cube {
private:
	Face face[NUM_FACES];
	Point3D center;
	int x, y, z;

public:
	Cube();
	Cube(Face f0, Face f1, Face f2, Face f3, Face f4, Face f5);
	
	void set(Cube &cube);
	void setFace(Face f0, Face f1, Face f2, Face f3, Face f4, Face f5);
	
	void draw();
	void rotate(Vector3D u, float angle);
	
	Point3D getCenter();
	
	void setIndex(int, int, int);
	void setX(int);
	void setY(int);
	void setZ(int);
	int getX();
	int getY();
	int getZ();
};


#endif //RUBIK_CUBE_H
