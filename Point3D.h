#ifndef RUBIK_POINT3D_H
#define RUBIK_POINT3D_H

class Point3D {
	private:
		float x, y, z;

	public:
		Point3D();
		Point3D(float, float, float);
		Point3D(const Point3D&);
		~Point3D();
		
		void set(float, float, float);
		void set(const Point3D&);
		float getX();
		float getY();
		float getZ();
		void rotate(float, float, float, float);
};

#endif
