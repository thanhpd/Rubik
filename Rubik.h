#ifndef RUBIK_RUBIK_H
#define RUBIK_RUBIK_H

#include <iostream>
#include "Cube.h"
#include "Image.h"
#include "Constant.h"

using namespace std;

class Rubik {
	private:
		int n, rotationNumber;
		Cube ***cube;
		Point3D min, max;
		
		void initCubeMatrix();
		GLuint loadTexture(string);
		GLuint loadTexture(Image*);
		bool checkSlice(int, int, int, int, int, int);
    
	public:
		Rubik();
		Rubik(int);
		~Rubik();
		void init();
		void draw();
		void rotate(int, int, float);
		Cube getCube(int, int, int);
		void invertSlice(int, int, int);
		Point3D getMinPoint();
		Point3D getMaxPoint();
		void setSize(int);
		int getSize();
		void setRotationNumber(int);
		int getRotationNumber();
		bool isCorrect();
};

#endif //RUBIK_RUBIK_H
