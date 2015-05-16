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

public:
    Cube();

    Cube(Face f0, Face f1, Face f2, Face f3, Face f4, Face f5);

    void set(Face f0, Face f1, Face f2, Face f3, Face f4, Face f5);

    void set(const Cube &cube);

    void draw();

    void rotate(Vector3D u, float angle);

    Point3D getCenter();
};


#endif //RUBIK_CUBE_H
