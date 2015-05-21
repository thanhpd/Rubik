#ifndef RUBIK_VECTOR3D_H
#define RUBIK_VECTOR3D_H

#include "Point3D.h"

class Vector3D {
	private:
		float x, y, z;

	public:
		Vector3D();
		Vector3D(float, float, float);
		Vector3D(const Vector3D &other);
		~Vector3D();
		
		void set(float, float, float);
		void set(const Vector3D &other);
		float getX();
		float getY();
		float getZ();
		
		/* function to return the length of the vector */
		float length();
		
		/* function to set a vector from 2 points. This vector will be set to equal
		to vector AB */
		void setDiff(Point3D A, Point3D B);
		
		/* function to re-length the vector to 1, with the same direction */
		void normalize();
		
		/* calculate the dot product of this vector and other vector */
		float dot(Vector3D other);
		
		/* calculate the cross product of this vector and other vector */
		Vector3D cross(Vector3D other);
};

#endif
