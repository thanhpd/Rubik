#include "Rubik.h"

GLuint Rubik::loadTexture(Image *image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    return textureId;
}

Rubik::Rubik() {
    init();
}

void Rubik::init() {
    Point3D p[2][2][2];
    Face faceX[2][1][1], faceY[1][2][1], faceZ[1][1][2];

    GLuint white_textureId;
    GLuint red_textureId;
    GLuint blue_textureId;
    GLuint green_textureId;
    GLuint yellow_textureId;
    GLuint orange_textureId;
    GLuint gray_textureId;

    /* bind each color image with an id */
    Image *image1 = loadBMP("/Users/GiapNV/ClionProjects/Rubik/red.bmp");
    red_textureId = loadTexture(image1);
    delete image1;


    Image *image = loadBMP("/Users/GiapNV/ClionProjects/Rubik/white.bmp");
    white_textureId = loadTexture(image);
    delete image;

    Image *image2 = loadBMP("/Users/GiapNV/ClionProjects/Rubik/blue.bmp");
    blue_textureId = loadTexture(image2);
    delete image2;

    Image *image3 = loadBMP("/Users/GiapNV/ClionProjects/Rubik/orange.bmp");
    orange_textureId = loadTexture(image3);
    delete image3;

    Image *image4 = loadBMP("/Users/GiapNV/ClionProjects/Rubik/green.bmp");
    green_textureId = loadTexture(image4);
    delete image4;

    Image *image5 = loadBMP("/Users/GiapNV/ClionProjects/Rubik/yellow.bmp");
    yellow_textureId = loadTexture(image5);
    delete image5;

    Image *image6 = loadBMP("/Users/GiapNV/ClionProjects/Rubik/gray.bmp");
    gray_textureId = loadTexture(image6);
    delete image6;

    /* initialize all point in the rubik */
    for (int x = 0; x < 2; x++)
        for (int y = 0; y < 2; y++)
            for (int z = 0; z < 2; z++)
                p[x][y][z].set((float) x - 1.5f, (float) y - 1.5f, (float) z - 1.5f);

    /* initilize all Face that perpendicular with x-axis */
    for (int x = 0; x < 2; x++)
        for (int y = 0; y < 1; y++)
            for (int z = 0; z < 1; z++)
                if (x == 0)
                    faceX[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x][y + 1][z + 1], p[x][y][z + 1],
                                       orange_textureId);
                else if (x == 1)
                    faceX[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x][y + 1][z + 1], p[x][y][z + 1], red_textureId);
                else
                    faceX[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x][y + 1][z + 1], p[x][y][z + 1], gray_textureId);

    /* initilize all Face that perpendicular with y-axis */
    for (int x = 0; x < 1; x++)
        for (int y = 0; y < 2; y++)
            for (int z = 0; z < 1; z++)
                if (y == 0)
                    faceY[x][y][z].set(p[x][y][z], p[x][y][z + 1], p[x + 1][y][z + 1], p[x + 1][y][z], blue_textureId);
                else if (y == 1)
                    faceY[x][y][z].set(p[x][y][z], p[x][y][z + 1], p[x + 1][y][z + 1], p[x + 1][y][z], green_textureId);
                else
                    faceY[x][y][z].set(p[x][y][z], p[x][y][z + 1], p[x + 1][y][z + 1], p[x + 1][y][z], gray_textureId);

    /* initilize all Face that perpendicular with z-axis */
    for (int x = 0; x < 1; x++)
        for (int y = 0; y < 1; y++)
            for (int z = 0; z < 2; z++)
                if (z == 0)
                    faceZ[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x + 1][y + 1][z], p[x + 1][y][z],
                                       yellow_textureId);
                else if (z == 1)
                    faceZ[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x + 1][y + 1][z], p[x + 1][y][z], white_textureId);
                else
                    faceZ[x][y][z].set(p[x][y][z], p[x][y + 1][z], p[x + 1][y + 1][z], p[x + 1][y][z], gray_textureId);



    /* inititalize 27 SolidCubes of the Rubik */
    int x = 0, y = 0, z = 0;
//    for (int x = 0; x < 3; x++)
//        for (int y = 0; y < 3; y++)
//            for (int z = 0; z < 3; z++)
    cube[9 * x + 3 * y + z].set(faceX[x][y][z], faceX[x + 1][y][z], faceY[x][y][z], faceY[x][y + 1][z],
                                faceZ[x][y][z], faceZ[x][y][z + 1]);

}

void Rubik::draw() {
//    for (int i = 0; i < 27; i++)
    cube[0].draw();
}

void Rubik::rotate(int face, float angle) {
    Vector3D u;

    /* to rotate a face of Rubik, we only need to rotate 9 SolidCube of that face */
    if (face == RED) {
        u.set(1.0, 0.0, 0.0);
        for (int i = 0; i < 27; i++)
            if (cube[i].getCenter().getX() > 0.5)
                cube[i].rotate(u, angle);
    }

    if (face == GREEN) {
        u.set(0.0, 1.0, 0.0);
        for (int i = 0; i < 27; i++)
            if (cube[i].getCenter().getY() > 0.5)
                cube[i].rotate(u, angle);
    }

    if (face == WHITE) {
        u.set(0.0, 0.0, 1.0);
        for (int i = 0; i < 27; i++)
            if (cube[i].getCenter().getZ() > 0.5)
                cube[i].rotate(u, angle);
    }

    if (face == ORANGE) {
        u.set(-1.0f, 0.0, 0.0);
        for (int i = 0; i < 27; i++)
            if (cube[i].getCenter().getX() < -0.5)
                cube[i].rotate(u, angle);
    }

    if (face == BLUE) {
        u.set(0.0, -1.0f, 0.0);
        for (int i = 0; i < 27; i++)
            if (cube[i].getCenter().getY() < -0.5)
                cube[i].rotate(u, angle);
    }

    if (face == YELLOW) {
        u.set(0.0, 0.0, -1.0f);
        for (int i = 0; i < 27; i++)
            if (cube[i].getCenter().getZ() < -0.5)
                cube[i].rotate(u, angle);
    }
}


