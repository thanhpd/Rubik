#ifndef RUBIK_FACE_H
#define RUBIK_FACE_H

#ifdef __APPLE__

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include "Point3D.h"
#include "Vector3D.h"

#define NUM_POINTS 4

class Face {
	private:
	    Point3D vertex[NUM_POINTS];
	    GLuint textureId;
	    Point3D center;

	public:
	    Face();
	    Face(Point3D v0, Point3D v1, Point3D v2, Point3D v3, GLuint textureId);
	
	    void set(Point3D v0, Point3D v1, Point3D v2, Point3D v3, GLuint textureId);
	    void set(const Face &f);
	
	    void draw();
	    void rotate(Vector3D u, float angle);
	    Point3D getCenter();
};

#endif
