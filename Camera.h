#ifndef _CAMERA_H_
#define _CAMERA_H_

/** 
 * This class define a camera in the 3-D coordinate system. It describes how
 * a camera move in the scence and change the direction 
 **/
#include <GL/gl.h>
 
#include "Point3D.h"
#include "Vector3D.h"

class Camera{
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
		void set(Point3D, Point3D, Vector3D);
		void set(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
		void setShape(GLfloat, GLfloat, GLfloat, GLfloat);
		void setAspect(GLfloat);
		
		/* getters */
		void getShape(GLfloat&, GLfloat&, GLfloat&, GLfloat&);
		
		/* function to change the direction of the camera */
		void pitch(GLfloat);
		void roll(GLfloat);
		void yaw(GLfloat);
		
		/* change the position of the camera */
		void slide(GLfloat, GLfloat, GLfloat);	
		void rotAxes(Vector3D&, Vector3D&, GLfloat);
		
		/* set the first default camera */ 
	    void setDefaultCamera(); 	
	    
	    /* change the camera such as the rubik look like rotate about the 
	    horizontal axis that parallels with the view plane */
	    void up(float);
	    
	    /* change the camera such as the rubik look like rotate about the vertical
	    axis */
	    void right(float);
	    
	    /* change the camera such as the rubik look like rotate about the aixs that
	    perpendicular with the view plane */
	    void rotate(float);
};

#endif
