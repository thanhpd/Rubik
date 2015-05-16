#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>

#ifdef __APPLE__

#include <GLUT/glut.h>
#include "Rubik.h"

#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

const int screenWidth = 640;
const int screenHeight = 480;
const char *screenTitle = "Rubik";

Rubik myRubik;

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    myRubik.draw();

    glFlush();
    glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {

}

void mySpecial(int key, int x, int y) {

}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);

    glutCreateWindow(screenTitle);
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);

    myRubik.init();

    glutMainLoop();

    return 0;
}
