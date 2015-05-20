#include "Rubik.h"

Rubik::Rubik() {
	n = 3;
	init();
}

Rubik::Rubik(int n) {
	this->n = n;
	init();
}

Rubik::~Rubik(){
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			delete cube[i][j];
		}
		delete cube[i];
	}
	delete cube;
}

void Rubik::initCubeMatrix(){
	cube = new Cube**[n];
	for (int i = 0; i < n; i++){
		cube[i] = new Cube*[n];
		for (int j = 0; j < n; j++){
			cube[i][j] = new Cube[n];
		}
	}
}

GLuint Rubik::loadTexture(Image *image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    return textureId;
}

GLuint Rubik::loadTexture(string imageName){
	Image *image = loadBMP(imageName.c_str());
	GLuint id = loadTexture(image);
	delete image;
	return id;
}

void Rubik::init() {
	initCubeMatrix();
	
    /* bind each color image with an id */
    GLuint whiteId = loadTexture("white.bmp");
    GLuint redId = loadTexture("red.bmp");
    GLuint blueId = loadTexture("blue.bmp");
    GLuint orangeId = loadTexture("orange.bmp");
    GLuint greenId = loadTexture("green.bmp");
    GLuint yellowId = loadTexture("yellow.bmp");
    GLuint grayId = loadTexture("gray.bmp");

	Point3D p[n + 1][n + 1][n + 1];
	Face faceX[n + 1][n][n], faceY[n][n + 1][n], faceZ[n][n][n + 1];
	
	/* initialize all points in the rubik */
	double m = n / 2.0;
	for (int x = 0; x <= n; x++)
		for (int y = 0; y <= n; y++)
			for (int z = 0; z <= n; z++)
				p[x][y][z].set((float) x - m, (float) y - m, (float) z - m);
	min.set(-m, -m, -m);
	max.set(m, m, m);

	GLuint textureId;
    /* initilize all faces that perpendicular with x-axis */
	for (int x = 0; x <= n; x++)
		for (int y = 0; y < n; y++)
			for (int z = 0; z < n; z++){
				if (x == 0) textureId = orangeId;
				else if (x == n) textureId = redId;
				else textureId = grayId;
				faceX[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x][y + 1][z + 1], p[x][y][z + 1], textureId);
			}

    /* initilize all faces that perpendicular with y-axis */
	for (int x = 0; x < n; x++)
		for (int y = 0; y <= n; y++)
			for (int z = 0; z < n; z++){
				if (y == 0) textureId = blueId;
				else if (y == n) textureId = greenId;
				else textureId = grayId;
				faceY[x][y][z].set(p[x][y][z], p[x][y][z + 1], p[x + 1][y][z + 1], p[x + 1][y][z], textureId);
			}
			
    /* initilize all faces that perpendicular with z-axis */
	for (int x = 0; x < n; x++)
		for (int y = 0; y < n; y++)
			for (int z = 0; z <= n; z++){
				if (z == 0) textureId = yellowId;
				else if (z == n) textureId = whiteId;
				else textureId = grayId;
				faceZ[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x + 1][y + 1][z], p[x + 1][y][z], textureId);
			}

	/* inititalize n*n*n cubes of the Rubik */
	for (int x = 0; x < n; x++)
		for (int y = 0; y < n; y++)
			for (int z = 0; z < n; z++){
				cube[x][y][z].setFace(faceX[x][y][z], faceX[x + 1][y][z], faceY[x][y][z], faceY[x][y + 1][z],
										faceZ[x][y][z], faceZ[x][y][z + 1]);
				cube[x][y][z].setIndex(x, y, z);
			}
	
	/* initialize the number of rotation */
	rotationNumber = 90;
}

void Rubik::draw() {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				cube[i][j][k].draw();
}

Cube Rubik::getCube(int i, int j, int k){
	return cube[i][j][k];
}

void Rubik::rotate(int name, int value, float angle){
	Vector3D u;
	int i1, i2, j1, j2, k1, k2;
	
	switch(name){
		case SLICE_X:
			i1 = i2 = value;
			j1 = 0; j2 = n - 1;
			k1 = 0; k2 = n - 1;
			u.set(1, 0, 0);
			break;
		case SLICE_Y:
			i1 = 0; i2 = n - 1;
			j1 = j2 = value;
			k1 = 0; k2 = n - 1;
			u.set(0, 1, 0);
			break;
		case SLICE_Z:
			i1 = 0; i2 = n - 1;
			j1 = 0; j2 = n - 1;
			k1 = k2 = value;
			u.set(0, 0, 1);
			break;
	}
	
	for (int i = i1; i <= i2; i++)
		for (int j = j1; j <= j2; j++)
			for (int k = k1; k <= k2; k++)
				cube[i][j][k].rotate(u, angle);
}

void Rubik::invertSlice(int name, int value, int dir){
	Cube tem[n][n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (name == SLICE_X) tem[i][j].set(cube[value][i][j]);
			else if (name == SLICE_Y) tem[i][j].set(cube[i][value][j]);
			else if (name == SLICE_Z) tem[i][j].set(cube[i][j][value]);
	
	if (dir == COUNTER_CLOCKWISE){
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (name == SLICE_X) cube[value][i][j].set(tem[j][n-i-1]);
				else if (name == SLICE_Y) cube[i][value][j].set(tem[n-j-1][i]);
				else if (name == SLICE_Z) cube[i][j][value].set(tem[j][n-i-1]);
	}else{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (name == SLICE_X) cube[value][i][j].set(tem[n-j-1][i]);
				else if (name == SLICE_Y) cube[i][value][j].set(tem[j][n-i-1]);
				else if (name == SLICE_Z) cube[i][j][value].set(tem[n-j-1][i]);
	}
}

Point3D Rubik::getMinPoint(){
	return min;
}

Point3D Rubik::getMaxPoint(){
	return max;
}

void Rubik::setSize(int n){
	this->n = n;
}

int Rubik::getSize(){
	return n;
}

void Rubik::setRotationNumber(int rotationNumber){
	this->rotationNumber = rotationNumber;
}

int Rubik::getRotationNumber(){
	return rotationNumber;
}

bool Rubik::checkSlice(int x0, int x1, int y0, int y1, int z0, int z1){
	bool checkX, checkY, checkZ;
	checkX = checkY = checkZ = true;
	int tx = cube[x0][y0][z0].getX();
	int ty = cube[x0][y0][z0].getY();
	int tz = cube[x0][y0][z0].getZ();
	for (int x = x0; x <= x1; x++)
		for (int y = y0; y <= y1; y++)
			for (int z = z0; z <= z1; z++){
				checkX &= (cube[x][y][z].getX() == tx);
				checkY &= (cube[x][y][z].getY() == ty);
				checkZ &= (cube[x][y][z].getZ() == tz);
				if (!(checkX || checkY || checkZ))
					return false;				
			}
	return true;
}

bool Rubik::isCorrect(){
	if (	! checkSlice(0, 0, 0, n - 1, 0, n - 1)			// check slice x = 0
		 ||	! checkSlice(n - 1, n - 1, 0, n - 1, 0, n - 1)	// check slice x = n-1
		 ||	! checkSlice(0, n - 1, 0, 0, 0, n - 1)			// check slice y = 0
		 ||	! checkSlice(0, n - 1, n - 1, n - 1, 0, n - 1)	// check slice y = n-1
		 ||	! checkSlice(0, n - 1, 0, n - 1, 0, 0)			// check slice z = 0
		 ||	! checkSlice(0, n - 1, 0, n - 1, n - 1, n - 1)	// check slice z = n-1
	) return false;
	
	return true;
}
