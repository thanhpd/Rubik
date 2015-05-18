#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "Point3D.h"
#include "Vector3D.h"
#include "Camera.h"
#include "Rubik.h"

using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

const int screenWidth = 600;
const int screenHeight = 600;
const char *screenTitle = "Rubik";
const int rotationNum = 90;
const double deltaAngle = 90.0 / rotationNum;

Camera myCam;

Rubik myRubik;
int n = 5;
Point3D minPoint, maxPoint;

Point3D nearPoint, farPoint;

Point3D clickPoint;
int sliceName, cubeX, cubeY, cubeZ;
double planeValue;

int rotateSliceName, rotateSliceValue;
bool isCheckingRubik, isRotating;
double rotateAngle;

bool isCheckingCamera;
int camX, camY;

void myInit(){
	myRubik.setSize(n);
    myRubik.init();
    minPoint.set(myRubik.getMinPoint());
    maxPoint.set(myRubik.getMaxPoint());
    
    double m = 2 * n;
 	myCam.set(m, m, m, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
 	myCam.setShape(45.0f, 1.0, 0.1f, 100.0f);
 	
 	isCheckingRubik = isRotating = isCheckingCamera = false;
}

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    myRubik.draw();

    glFlush();
    glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch(key){
		case '+': myCam.slide(0, 0, -0.2); break;
		case '-': myCam.slide(0, 0, 0.2); break;
		case 'c': myCam.rotate_clockwise(5.0); break;
		case 'v': myCam.rotate_clockwise(-5.0); break;
	}
	
	glutPostRedisplay();
}

void findNearAndFarPoint(int x, int y){
	double matModelView[16], matProjection[16], winX, winY;
	int viewport[4];
	
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);	
	winX = (double) x;
	winY = viewport[3] - y;
	
	double tx, ty, tz;
	gluUnProject(winX, winY, 0.0, matModelView, matProjection, viewport, &tx, &ty, &tz);
	nearPoint.set(tx, ty, tz);
	gluUnProject(winX, winY, 1.0, matModelView, matProjection, viewport, &tx, &ty, &tz);
	farPoint.set(tx, ty, tz);
}

bool isInRange(double x, double a, double b){
	return (x > a && x < b);
}

double manhattanDistance(Point3D a, Point3D b){
	return abs(a.getX() - b.getX()) + abs(a.getY() - b.getY()) + abs(a.getZ() - b.getZ());
}

void findNearestCube(const Point3D &p, int x1, int x2, int y_1, int y2, int z1, int z2, int &resX, int &resY, int &resZ){
	resX = x1; resY = y_1; resZ = z1;
	double dist, minDist;
	minDist = manhattanDistance(p, myRubik.getCube(x1, y_1, z1).getCenter());
	for (int x = x1; x <= x2; x++)
		for (int y = y_1; y <= y2; y++)
			for (int z = z1; z <= z2; z++){
				dist = manhattanDistance(p, myRubik.getCube(x, y, z).getCenter());
				if (dist < minDist){
					minDist = dist;
					resX = x; resY = y; resZ = z;
				}
			}
}

/**
	return true ~ the ray cut the rubik, false ~ not cut
	intersection	: the intersection point between the ray and the rubik face
	slideName		: name of slide cutting the ray ( SLICE_X | SLICE_Y | SLICE_Z )
	cubeX&Y&Z		: indexs of the cube cutting the ray
	planeValue		: value of the plane cutting the ray (planeName = slideName)
*/
bool findIntersectionWithRubikFace(Point3D &intersection, int &slideName, int &cubeX, int &cubeY, int &cubeZ, double &planeValue){
	double dx, dy, dz, t, tx, ty, tz, t1, t2, x, y, z;
	
	// init the line cross near point and far point
	dx = farPoint.getX() - nearPoint.getX();
	dy = farPoint.getY() - nearPoint.getY();
	dz = farPoint.getZ() - nearPoint.getZ();
	
	t = INF;
	// find intersection with slice X
	if (dx != 0){
		t1 = (minPoint.getX() - nearPoint.getX()) / dx;
		t2 = (maxPoint.getX() - nearPoint.getX()) / dx;
		tx = INF;
		if (isInRange(t1, 0, 1) && t1 < tx)
			tx = t1;
		if (isInRange(t2, 0, 1) && t2 < tx)
			tx = t2;
		if (tx != INF && tx < t){
			x = (tx == t1) ? minPoint.getX() : maxPoint.getX();
			y = nearPoint.getY() + tx * dy;
			z = nearPoint.getZ() + tx * dz;
			if (isInRange(y, minPoint.getY(), maxPoint.getY()) && isInRange(z, minPoint.getZ(), maxPoint.getZ())){
				t = tx;
				intersection.set(x, y, z);
				slideName = SLICE_X;
				cubeX = (tx == t1) ? 0 : n-1;
				findNearestCube(intersection, cubeX, cubeX, 0, n-1, 0, n-1, cubeX, cubeY, cubeZ);
				planeValue = x;
			}
		}
	}
	
	// find intersection with slice Y
	if (dy != 0){
		t1 = (minPoint.getY() - nearPoint.getY()) / dy;
		t2 = (maxPoint.getY() - nearPoint.getY()) / dy;
		ty = INF;
		if (isInRange(t1, 0, 1) && t1 < ty)
			ty = t1;
		if (isInRange(t2, 0, 1) && t2 < ty)
			ty = t2;
		if (ty != INF && ty < t){
			x = nearPoint.getX() + ty * dx;
			y = (ty == t1) ? minPoint.getY() : maxPoint.getY();
			z = nearPoint.getZ() + ty * dz;
			if (isInRange(x, minPoint.getX(), maxPoint.getX()) && isInRange(z, minPoint.getZ(), maxPoint.getZ())){
				t = ty;
				intersection.set(x, y, z);
				slideName = SLICE_Y;
				cubeY = (t == t1) ? 0 : n-1;
				findNearestCube(intersection, 0, n-1, cubeY, cubeY, 0, n-1, cubeX, cubeY, cubeZ);
				planeValue = y;
			}
		}
	}
	
	// find intersection with slice Z
	if (dz != 0){
		t1 = (minPoint.getZ() - nearPoint.getZ()) / dz;
		t2 = (maxPoint.getZ() - nearPoint.getZ()) / dz;
		tz = INF;
		if (isInRange(t1, 0, 1) && t1 < tz)
			tz = t1;
		if (isInRange(t2, 0, 1) && t2 < tz)
			tz = t2;
		if (tz != INF && tz < t){
			x = nearPoint.getX() + tz * dx;
			y = nearPoint.getY() + tz * dy;
			z = (tz == t1) ? minPoint.getZ() : maxPoint.getZ();
			if (isInRange(x, minPoint.getX(), maxPoint.getX()) && isInRange(y, minPoint.getY(), maxPoint.getY())){
				t = tz;
				intersection.set(x, y, z);
				slideName = SLICE_Z;
				cubeZ = (t == t1) ? 0 : n-1;
				findNearestCube(intersection, 0, n-1, 0, n-1, cubeZ, cubeZ, cubeX, cubeY, cubeZ);
				planeValue = z;
			}
		}
	}
	
	if (t != INF)
		return true;
	else
		return false;
}

void myMouse(int button, int state, int x, int y){
	if (state == GLUT_DOWN){ // mouse click
		findNearAndFarPoint(x, y);
		Point3D p;
		int sName, cx, cy, cz;
		double pValue;
		if (findIntersectionWithRubikFace(p, sName, cx, cy, cz, pValue)){ // click in rubik
			if (!isRotating){
				isCheckingRubik = true;
				clickPoint.set(p);
				sliceName = sName;
				cubeX = cx; cubeY = cy; cubeZ = cz;
				planeValue = pValue;
			}
		}else { // click in backgroud
			isCheckingCamera = true;
			camX = x; camY = y;
		}
	}else{ // mouse release
		isCheckingRubik = false;
		isCheckingCamera = false;
	}
}

Point3D findIntersectionWithPlane(int name, double value){
	double dx, dy, dz, t, px, py, pz;

	// init the line cross near point and far point
	dx = farPoint.getX() - nearPoint.getX();
	dy = farPoint.getY() - nearPoint.getY();
	dz = farPoint.getZ() - nearPoint.getZ();

	if (name == SLICE_X){
		t = (value - nearPoint.getX()) / dx;
		px = value;
		py = nearPoint.getY() + t * dy;
		pz = nearPoint.getZ() + t * dz;
	}else if (name == SLICE_Y){
		t = (value - nearPoint.getY()) / dy;
		px = nearPoint.getX() + t * dx;
		py = value;
		pz = nearPoint.getZ() + t * dz;
	}else{
		t = (value - nearPoint.getZ()) / dz;
		px = nearPoint.getX() + t * dx;
		py = nearPoint.getY() + t * dy;
		pz = value;
	}
	
	return Point3D(px, py, pz);
}

void rotateRubik(int num){
	if (num < rotationNum) {
		myRubik.rotate(rotateSliceName, rotateSliceValue, rotateAngle);
		glutPostRedisplay();
		glutTimerFunc(1, rotateRubik, num + 1);
	} else {
		isRotating = false;
		int dir = (rotateAngle == -deltaAngle) ? CLOCKWISE : COUNTER_CLOCKWISE;
		myRubik.invertSlice(rotateSliceName, rotateSliceValue, dir);
	}
}

void rotateRubik(Point3D a, Point3D b){
	double absX = abs(b.getX() - a.getX());
	double absY = abs(b.getY() - a.getY());
	double absZ = abs(b.getZ() - a.getZ());
	
	// -deltaAngle ~ CLOCKWISE ; deltaAngle ~ COUNTER_CLOCKWISE
	if (sliceName == SLICE_X){
		if (absZ > absY){
			rotateSliceName = SLICE_Y;
			rotateSliceValue = cubeY;
			if (cubeX == 0) rotateAngle = (b.getZ() > a.getZ()) ? deltaAngle : -deltaAngle;
			else rotateAngle = (b.getZ() > a.getZ()) ? -deltaAngle : deltaAngle;
		}else{
			rotateSliceName = SLICE_Z;
			rotateSliceValue = cubeZ;
			if (cubeX == 0) rotateAngle = (b.getY() > a.getY()) ? -deltaAngle : deltaAngle;
			else rotateAngle = (b.getY() > a.getY()) ? deltaAngle : -deltaAngle;
		}
	}else if (sliceName == SLICE_Y){
		if (absZ > absX){
			rotateSliceName = SLICE_X;
			rotateSliceValue = cubeX;
			if (cubeY == 0) rotateAngle = (b.getZ() > a.getZ()) ? -deltaAngle : deltaAngle;
			else rotateAngle = (b.getZ() > a.getZ()) ? deltaAngle : -deltaAngle;
		}else{
			rotateSliceName = SLICE_Z;
			rotateSliceValue = cubeZ;
			if (cubeY == 0) rotateAngle = (b.getX() > a.getX()) ? deltaAngle : -deltaAngle;
			else rotateAngle = (b.getX() > a.getX()) ? -deltaAngle : deltaAngle;
		}
	}else{
		if (absX > absY){
			rotateSliceName = SLICE_Y;
			rotateSliceValue = cubeY;
			if (cubeZ == 0) rotateAngle = (b.getX() > a.getX()) ? -deltaAngle : deltaAngle;
			else rotateAngle = (b.getX() > a.getX()) ? deltaAngle : -deltaAngle;
		}else{
			rotateSliceName = SLICE_X;
			rotateSliceValue = cubeX;
			if (cubeZ == 0) rotateAngle = (b.getY() > a.getY()) ? deltaAngle : -deltaAngle;
			else rotateAngle = (b.getY() > a.getY()) ? -deltaAngle : deltaAngle;
		}
	}

	glutTimerFunc(1, rotateRubik, 0);
}

void myMotion(int x, int y){
	findNearAndFarPoint(x, y);
	
	if (isCheckingRubik){
		Point3D p = findIntersectionWithPlane(sliceName, planeValue);
		if (manhattanDistance(p, clickPoint) > 0.7){ // start rotate
			isCheckingRubik = false;
			isRotating = true;
			rotateRubik(clickPoint, p);
		}
	}else if (isCheckingCamera){
		int dx = x - camX, dy = y - camY;
		myCam.up(dy * 0.5);
		myCam.right(dx * 0.5);
		camX = x; camY = y;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	
	glutCreateWindow(screenTitle);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	
	myInit();
	
	glutMainLoop();
	return 0;
}
