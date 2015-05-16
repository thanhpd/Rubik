#ifndef RUBIK_RUBIK_H
#define RUBIK_RUBIK_H


#include "Cube.h"
#include "Image.h"

const int RED = 22;
const int GREEN = 16;
const int WHITE = 14;
const int ORANGE = 4;
const int BLUE = 10;
const int YELLOW = 12;

class Rubik {
private:
    Cube cube[27];

    GLuint loadTexture(Image *image);

public:
    Rubik();

    void init();

    void draw();

    void rotate(int face, float angle);
};


#endif //RUBIK_RUBIK_H
