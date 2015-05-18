#ifndef _CAMERA_H_
#define _CAMERA_H_

/** 
 * This class define a camera in the 3-D coordinate system. It describes how
 * a camera move in the scence and change the direction 
 **/

#include <GL/gl.h>
 
#include "Point3D.h"
#include "Vector3D.h"

class Camera
{
private:
	Point3D eye;	
	Vector3D u, v, n;	
	GLdouble viewAngle, aspect, nearDist, farDist;
	
	/* this private method used for re-calculate the MODELVIEWS matrix after
	each modification of the camera */
	void setModelViewMatrix();
	
public:
	/* constructor and destructor */
	Camera();
	~Camera();
	
	/* setters */
	void set(Point3D _eye, Point3D _look, Vector3D _up);
	void set( GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat lookX, GLfloat lookY, 
		GLfloat lookZ, GLfloat upX, GLfloat upY, GLfloat upZ );
	void setShape(GLfloat vAng, GLfloat asp, GLfloat nearD, GLfloat farD);
	void setAspect(GLfloat asp);
	
	/* getters */
	void getShape(GLfloat &vAng, GLfloat &asp, GLfloat &nearD, GLfloat &farD);
	
	/* function to change the direction of the camera */
	void pitch(GLfloat angle); 
	void roll(GLfloat angle);  
	void yaw(GLfloat angle);
	
	/* change the position of the camera */
	void slide(GLfloat delU, GLfloat delV, GLfloat delN);	
	void rotAxes(Vector3D& a, Vector3D& b, GLfloat angle);
	
	/* set the first default camera */ 
    void setDefaultCamera(); 	
    
    /* change the camera such as the rubik look like rotate about the 
    horizontal axis that parallels with the view plane */
    void up(float _angle);
    
    /* change the camera such as the rubik look like rotate about the vertical
    axis */
    void right(float _angle);
    
    /* change the camera such as the rubik look like rotate about the aixs that
    perpendicular with the view plane */
    void rotate_clockwise(float _angle);
};

#endif
