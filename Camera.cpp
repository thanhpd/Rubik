/**
 * This file implements all the methods of class Camera which is defined int 
 * file "Camera.h"
 **/
 
#include "GL/glu.h" 

#include <cmath>
#include "Camera.h"

void Camera::setModelViewMatrix()
{
	float m[16]; 
    	Vector3D eVec(eye.getX(), eye.getY(), eye.getZ()); 
    	m[0] = u.getX(); m[4] = u.getY(); m[8]  = u.getZ(); m[12] = -eVec.dot(u); 
    	m[1] = v.getX(); m[5] = v.getY(); m[9]  = v.getZ(); m[13] = -eVec.dot(v); 
    	m[2] = n.getX(); m[6] = n.getY(); m[10] = n.getZ(); m[14] = -eVec.dot(n); 
    	m[3] = 0;   m[7] = 0;   m[11] = 0;   m[15] = 1.0; 
    	glMatrixMode(GL_MODELVIEW); 
    	glLoadMatrixf(m); 
}

void Camera:: set(Point3D _eye, Point3D _look, Vector3D _up)
{ // create a modelview matrix and send it to OpenGL
	eye.set(_eye); // store the given eye position
	n.set(_eye.getX() - _look.getX(), _eye.getY() - _look.getY(),
		_eye.getZ() - _look.getZ());
	// make n
	u.set(_up.cross(n)); // make u = up X n
	n.normalize(); 
	u.normalize(); // make them unit
	v.set(n.cross(u)); // make v = n X u
	setModelViewMatrix(); // tell OpenGL
}

/* constructor */
Camera::Camera()
{
	 setDefaultCamera(); // good default values here 
}

/* destructor */
Camera::~Camera()
{

}

void Camera::set( float eyeX, float eyeY, float eyeZ, float lookX, float lookY, 
	float lookZ, float upX, float upY, float upZ )
{
	Point3D _eye = Point3D( eyeX, eyeY, eyeZ );
	Point3D _look = Point3D( lookX, lookY, lookZ );

	Vector3D _up = Vector3D( upX, upY, upZ );

    	set(_eye, _look, _up);
}

void Camera::setShape(float vAngle, float asp, float nearD, float farD)
{
	viewAngle = vAngle; 
    	aspect = asp; 
    	nearDist = nearD; 
    	farDist = farD; 
    	glMatrixMode(GL_PROJECTION); 
    	glLoadIdentity(); 
    	gluPerspective(viewAngle, aspect, nearDist, farDist); 
    	glMatrixMode(GL_MODELVIEW); // set its mode back again 
}

void Camera::setAspect(float asp)
{
	aspect = asp;
}

void Camera::getShape(float &vAng, float &asp, float &nearD, float &farD)
{
	vAng = viewAngle;
	asp = aspect;
	nearD = nearDist;
	farD = farDist;
}

void Camera :: pitch (float angle)
{ 
	float cs = cos(3.14159265/180 * angle);
	//convert degrees to radians
	float sn = sin(3.14159265/180 * angle);
	Vector3D t(v);
	v.set(cs*t.getX() - sn*n.getX(), cs*t.getY() - sn*n.getY(), cs*t.getZ() - sn*n.getZ());
	n.set(sn*t.getX() + cs*n.getX(), sn*t.getY() + cs*n.getY(), sn*t.getZ() + cs*n.getZ());
	setModelViewMatrix();
}

void Camera :: roll (float angle)
{ // roll the camera through angle degrees
	float cs = cos(3.14159265/180 * angle);
	//convert degrees to radians
	float sn = sin(3.14159265/180 * angle);
	Vector3D t(u); // remember old u
	u.set(cs*t.getX() - sn*v.getX(), cs*t.getY() - sn*v.getY(), cs*t.getZ() - sn*v.getZ());
	v.set(sn*t.getX() + cs*v.getX(), sn*t.getY() + cs*v.getY(), sn*t.getZ() + cs*v.getZ());
	setModelViewMatrix();
}

void Camera :: yaw (float angle)
{ 
	float cs = cos(3.14159265/180 * angle);
	//convert degrees to radians
	float sn = sin(3.14159265/180 * angle);
	Vector3D t(n); 
	n.set(cs*t.getX() - sn*u.getX(), cs*t.getY() - sn*u.getY(), cs*t.getZ() - sn*u.getZ());
	u.set(sn*t.getX() + cs*u.getX(), sn*t.getY() + cs*u.getY(), sn*t.getZ() + cs*u.getZ());
	setModelViewMatrix();
}

void Camera::slide(float delU, float delV, float delN)
{
	float delx = delU*u.getX() + delV*v.getX() + delN*n.getX();
	float dely = delU*u.getY() + delV*v.getY() + delN*n.getY();
	float delz = delU*u.getZ() + delV*v.getZ() + delN*n.getZ();
	
	eye.set(eye.getX() + delx, eye.getY() + dely, eye.getZ() + delz);
	
	setModelViewMatrix();
}

void Camera::rotAxes(Vector3D& a, Vector3D& b, float angle)
{
	float ang = angle*M_PI/180;
	float C = cos(ang);
	float S = sin(ang);
	
	a.set(C * a.getX() + S * b.getX(), C * a.getY() + S * b.getY(),  
		C * a.getZ() + S * b.getZ());
	b.set(-S * a.getX() + C * b.getX(), -S * a.getY() + C * b.getY(), 
		-S * a.getZ() + C * b.getZ());
}

void Camera::setDefaultCamera()
{
	setShape(45.0f, 1.0f, 0.1f, 200.0f); // good default values here 
	
	Point3D eyePoint = Point3D( 10.0, 10.0, 10.0 );
	Point3D lookPoint = Point3D( 0.0, 0.0, 0.0 );
	Vector3D upVector = Vector3D( 0, 1, 0 );

	set( eyePoint, lookPoint, upVector );
}

void Camera::up(float _angle)
{
	Vector3D temp(eye.getX(), eye.getY(), eye.getZ());
	float dist = temp.length();
	eye.set(0, 0, 0);
	pitch(_angle);
	slide(0, 0, dist);
}

void Camera::right(float _angle)
{
	Vector3D temp(eye.getX(), eye.getY(), eye.getZ());
	float dist = temp.length();
	eye.set(0, 0, 0);
	yaw(_angle);
	slide(0, 0, dist);	
}

void Camera::rotate_clockwise(float _angle)
{
	Vector3D temp(eye.getX(), eye.getY(), eye.getZ());
	float dist = temp.length();
	eye.set(0, 0, 0);
	roll(_angle);
	slide(0, 0, dist);
}

