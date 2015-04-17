/* Module      : main.cpp
 * Author      : Giap Nguyen
 * Code        : 12020110
 * Email       : nvangiap5894@gmail.com
 * Course      : Computer Graphics
 *
 * Description :    
 *
 * 1. Load a 3D face of a human from a file and display the face.
 * The structure of the file is as follows:
 *
 * The number of vertices
 * Coordinates of vertices
 * The number of polygon
 * indexes of vertices of polygons
 *
 * 2. Implement pitch, roll and yaw of the camera. Links with keyboard events
 *
 * Date        : 12/4/2015
 *
 * History:
 * Revision      Date          Changed By
 * --------      ----------    ----------
 * 01.00         12/4/15        Giap Nguyen
 * First release.
 *
 */

/* -- INCLUDE FILES ------------------------------------------------------ */
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

/* -- CONSTANTS ---------------------------------------------------------- */
const int screenWidth = 640;
const int screenHeight = 480;
const int Max = 2000;

/* ----------------------------------------------------------------------- */
/* Function    : void myInit( void )
 *
 * Description : Initialize OpenGL and the window where things will be
 *               drawn.
 *
 * Parameters  : void
 *
 * Returns     : void
 */

void myInit(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);
    //    glMatrixMode(GL_PROJECTION);
    //    glLoadIdentity();
    //    gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}

/* ----------------------------------------------------------------------- */
/* Function    : void setWindow(GLdouble left, GLdouble right, GLdouble bottom,
 * GLdouble top)
 *
 * Description : The function setWindow sets the world window size.
 *
 * Parameters  :
 *
 * Returns     : void
 */

void setWindow(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top,
               GLdouble n, GLdouble f) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, n, f);
}

float zoom = 1.0f, rootx, rooty, tx, ty;

/* ----------------------------------------------------------------------- */
/* Function    : void myDisplay( void )
 *
 * Description : This function gets called everytime the window needs to
 *               be redrawn.
 *
 * Parameters  : void
 *
 * Returns     : void
 */

void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setWindow(-10, 15, -10, 15, -10, 15);
    glColor3f(0.0, 0.0, 1.0);

    glScalef(zoom, zoom, 0);
    glTranslatef(tx, ty, 0.0);
    glRotatef(rootx, 1.0, 0.0, 0.0);
    glRotatef(rooty, 0.0, 1.0, 0.0);
    glPushMatrix();

    float A[Max][3];
    float B[Max][3];
    std::ifstream data("face.dat");
    int n, m;
    if (!data) {
        std::cout << "Don't open file" << std::endl;

    } else {
        data >> n;
        for (int i = 0; i < n; i++) {
            data >> A[i][0] >> A[i][1] >> A[i][2];
            B[i][0] = -A[i][0];
            B[i][1] = A[i][1];
            B[i][2] = A[i][2];
        }
        data >> m;
        for (int i = 0; i < m; i++) {
            int x, y, z;
            data >> x >> y >> z;
            glBegin(GL_LINE_STRIP);
            glVertex3f(A[x][0], A[x][1], A[x][2]);
            glVertex3f(A[y][0], A[y][1], A[y][2]);
            glVertex3f(A[z][0], A[z][1], A[z][2]);
            glEnd();
            glBegin(GL_LINE_STRIP);
            glVertex3f(B[x][0], B[x][1], B[x][2]);
            glVertex3f(B[y][0], B[y][1], B[y][2]);
            glVertex3f(B[z][0], B[z][1], B[z][2]);
            glEnd();
        }
        glFlush();
    }
    data.close();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(0.0, (GLfloat)w / (GLfloat)h, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'm':
            zoom = zoom - 0.1;
            glutPostRedisplay();
            break;
        case 'n':
            zoom = zoom + 0.1;
            glutPostRedisplay();
            break;
        case 'q':
            rootx = (rootx + 1);
            glutPostRedisplay();
            break;
        case 'z':
            rootx = (rootx - 1);
            glutPostRedisplay();
            break;
        case 'e':
            rooty = (rooty + 1);
            glutPostRedisplay();
            break;
        case 'c':
            rooty = (rooty - 1);
            glutPostRedisplay();
            break;
        case 'w':
            tx = tx;
            ty = ty + 0.1;
            glutPostRedisplay();
            break;
        case 's':
            tx = tx;
            ty = ty - 0.1;
            glutPostRedisplay();
            break;
        case 'd':
            tx = tx + 0.1;
            ty = ty;
            glutPostRedisplay();
            break;
        case 'a':
            tx = tx - 0.1;
            ty = ty;
            glutPostRedisplay();
            break;
        case 'x':
            exit(1);
            break;
        default:
            break;
    }
}

/* ----------------------------------------------------------------------- */
/* Function    : int main( int argc, char** argv )
 *
 * Description : This is the main function. It sets up the rendering
 *               context, and then reacts to user events.
 *
 * Parameters  : int argc     : Number of command-line arguments.
 *               char *argv[] : Array of command-line arguments.
 *
 * Returns     : int : Return code to pass to the shell.
 */

int main(int argc, char* argv[]) {
    // Initialize GLUT.
    glutInit(&argc, argv);
    // Set the mode to draw in.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // Set the window size in screen pixels.
    glutInitWindowSize(screenWidth, screenHeight);
    // Set the window position in screen pixels.
    glutInitWindowPosition(100, 150);
    // Create the window.
    glutCreateWindow("Lab 7. Press 'w-a-s-d' to move; 'q-z-e-c' to rotate; 'm-n' to zoom and 'x' to exit!");
    // Set the callback funcion to call when we need to draw something.
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(keyboard);
    // Initialize some things.
    myInit();
    // Now that we have set everything up, loop responding to events.
    glutMainLoop();
}