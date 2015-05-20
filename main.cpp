#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <ctime>
#include "Point3D.h"
#include "Vector3D.h"
#include "Camera.h"
#include "Rubik.h"
#include "Help.h"

using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glui.h>
#endif

const int screenWidth = 800;
const int screenHeight = 600;
const int screenPosX = 100;
const int screenPosY = 100;
const char *screenTitle = "Rubik";

Camera myCam;

Rubik myRubik;
int n = 3;
Point3D minPoint, maxPoint;

Point3D nearPoint, farPoint;

Point3D clickPoint;
int sliceName, cubeX, cubeY, cubeZ;
double planeValue;

int rotateSliceName, rotateSliceValue;
bool isCheckingRubik, isRotating;
double rotateAngle, rotatedAngle;

bool initCube;
bool isCheckingCamera;
int camX, camY;

bool isShuffling;
int shuffleNum, shuffleCounter;

Help helper;

/** Live variables passed into GLUI **/
int size = 2;
int mainWindow;
int showRadio = 1;
int cubeSize = 3;
int timer = 5;
float speed = 2;
int enableSound = 1;
float cubeRotate = 1.0;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float objectPos[] = { 0.0, 0.0, 0.0 };

/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui, *_startScene;
GLUI_Spinner *speedSpinner;
GLUI_Panel *newGamePanel, *setPanel;
GLUI *gluiMain, *gluiSub;
bool gluiMainShow, gluiSubShow;
//GLUI_Spinner *speedSpinner;
//GLUI_Panel *newGamePanel, *setPanel;
GLUI_RadioGroup *modeGroup;
GLUI_Spinner *spinnerTimer;
GLUI_StaticText *timeRemain;

bool fullscreen = false;
bool timeModeGame = false;
bool timeOutGame = false;

//int FPS = 60;
int currentTime = 0;

/** User IDs for callbacks **/
#define GAME_MODE_ID 200
#define CLASSIC_MODE_ID 201
#define TIME_MODE_ID 202
#define CUBE_SIZE_ID 300
#define START_NEW_GAME_ID 199
#define RESET_GAME_ID 198
#define RESTART_GAME_ID 197
#define HELP_ID 100
#define DISABLE_ID 400
#define ENABLE_ID 401
#define HIDE_ID 402
#define SHOW_ID 403
#define SPEED_ID 500
#define INVOKE_ID 501

void myInit(){
	myRubik.setSize(n);
    myRubik.init();
    minPoint.set(myRubik.getMinPoint());
    maxPoint.set(myRubik.getMaxPoint());
    
    double m = 2 * n;
 	myCam.set(m, m, m, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
 	myCam.setShape(45.0f, 1.0, 0.1f, 100.0f);
 	
 	isCheckingRubik = isRotating = isCheckingCamera = isShuffling = initCube = false;
}

void myDisplay() {;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    myRubik.draw();
    
    if(!initCube) {
    	myCam.right(0);
		initCube = true;	
    }
    
	
	/** Draw help screen **/
//	helper.set(screenWidth, screenHeight);
//	helper.draw();
	
	if (isShuffling && gluiMain) gluiMain->disable();
	else gluiMain->enable();
	
	if (!timeModeGame) timeRemain->disable();
	
    glFlush();
    glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'f': 
			if(!fullscreen) {
	        	glutFullScreen();
	        	fullscreen = true;
	    	} else if(fullscreen){
	        	glutReshapeWindow(screenWidth, screenHeight);
	        	glutPositionWindow(screenPosX, screenPosY);
	        	fullscreen = false;
	        }
	        break;
	}
	
	glutPostRedisplay();
}

void findNearAndFarPoint(int x, int y){
	double matModelView[16], matProjection[16], winX, winY;
	int viewport[4];
	
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewport);	
	winX = (double) x;
	winY = viewport[3] - y;
	
	double tx, ty, tz;
	gluUnProject(winX, winY, 0.0, matModelView, matProjection, viewport, &tx, &ty, &tz);
	nearPoint.set(tx, ty, tz);
	gluUnProject(winX, winY, 1.0, matModelView, matProjection, viewport, &tx, &ty, &tz);
	farPoint.set(tx, ty, tz);
}

bool isInRange(double x, double a, double b){
	return (x > a && x < b);
}

double manhattanDistance(Point3D a, Point3D b){
	return abs(a.getX() - b.getX()) + abs(a.getY() - b.getY()) + abs(a.getZ() - b.getZ());
}

void findNearestCube(const Point3D &p, int x1, int x2, int y_1, int y2, int z1, int z2, int &resX, int &resY, int &resZ){
	resX = x1; resY = y_1; resZ = z1;
	double dist, minDist;
	minDist = manhattanDistance(p, myRubik.getCube(x1, y_1, z1).getCenter());
	for (int x = x1; x <= x2; x++)
		for (int y = y_1; y <= y2; y++)
			for (int z = z1; z <= z2; z++){
				dist = manhattanDistance(p, myRubik.getCube(x, y, z).getCenter());
				if (dist < minDist){
					minDist = dist;
					resX = x; resY = y; resZ = z;
				}
			}
}

/**
	return true ~ the ray cut the rubik, false ~ not cut
	intersection	: the intersection point between the ray and the rubik face
	slideName		: name of slide cutting the ray ( SLICE_X | SLICE_Y | SLICE_Z )
	cubeX&Y&Z		: indexs of the cube cutting the ray
	planeValue		: value of the plane cutting the ray (planeName = slideName)
*/
bool findIntersectionWithRubikFace(Point3D &intersection, int &slideName, int &cubeX, int &cubeY, int &cubeZ, double &planeValue){
	double dx, dy, dz, t, tx, ty, tz, t1, t2, x, y, z;
	
	// init the line cross near point and far point
	dx = farPoint.getX() - nearPoint.getX();
	dy = farPoint.getY() - nearPoint.getY();
	dz = farPoint.getZ() - nearPoint.getZ();
	
	t = INF;
	// find intersection with slice X
	if (dx != 0){
		t1 = (minPoint.getX() - nearPoint.getX()) / dx;
		t2 = (maxPoint.getX() - nearPoint.getX()) / dx;
		tx = INF;
		if (isInRange(t1, 0, 1) && t1 < tx)
			tx = t1;
		if (isInRange(t2, 0, 1) && t2 < tx)
			tx = t2;
		if (tx != INF && tx < t){
			x = (tx == t1) ? minPoint.getX() : maxPoint.getX();
			y = nearPoint.getY() + tx * dy;
			z = nearPoint.getZ() + tx * dz;
			if (isInRange(y, minPoint.getY(), maxPoint.getY()) && isInRange(z, minPoint.getZ(), maxPoint.getZ())){
				t = tx;
				intersection.set(x, y, z);
				slideName = SLICE_X;
				cubeX = (tx == t1) ? 0 : n-1;
				findNearestCube(intersection, cubeX, cubeX, 0, n-1, 0, n-1, cubeX, cubeY, cubeZ);
				planeValue = x;
			}
		}
	}
	
	// find intersection with slice Y
	if (dy != 0){
		t1 = (minPoint.getY() - nearPoint.getY()) / dy;
		t2 = (maxPoint.getY() - nearPoint.getY()) / dy;
		ty = INF;
		if (isInRange(t1, 0, 1) && t1 < ty)
			ty = t1;
		if (isInRange(t2, 0, 1) && t2 < ty)
			ty = t2;
		if (ty != INF && ty < t){
			x = nearPoint.getX() + ty * dx;
			y = (ty == t1) ? minPoint.getY() : maxPoint.getY();
			z = nearPoint.getZ() + ty * dz;
			if (isInRange(x, minPoint.getX(), maxPoint.getX()) && isInRange(z, minPoint.getZ(), maxPoint.getZ())){
				t = ty;
				intersection.set(x, y, z);
				slideName = SLICE_Y;
				cubeY = (t == t1) ? 0 : n-1;
				findNearestCube(intersection, 0, n-1, cubeY, cubeY, 0, n-1, cubeX, cubeY, cubeZ);
				planeValue = y;
			}
		}
	}
	
	// find intersection with slice Z
	if (dz != 0){
		t1 = (minPoint.getZ() - nearPoint.getZ()) / dz;
		t2 = (maxPoint.getZ() - nearPoint.getZ()) / dz;
		tz = INF;
		if (isInRange(t1, 0, 1) && t1 < tz)
			tz = t1;
		if (isInRange(t2, 0, 1) && t2 < tz)
			tz = t2;
		if (tz != INF && tz < t){
			x = nearPoint.getX() + tz * dx;
			y = nearPoint.getY() + tz * dy;
			z = (tz == t1) ? minPoint.getZ() : maxPoint.getZ();
			if (isInRange(x, minPoint.getX(), maxPoint.getX()) && isInRange(y, minPoint.getY(), maxPoint.getY())){
				t = tz;
				intersection.set(x, y, z);
				slideName = SLICE_Z;
				cubeZ = (t == t1) ? 0 : n-1;
				findNearestCube(intersection, 0, n-1, 0, n-1, cubeZ, cubeZ, cubeX, cubeY, cubeZ);
				planeValue = z;
			}
		}
	}
	
	if (t != INF)
		return true;
	else
		return false;
}

void myMouse(int button, int state, int x, int y){
	if (gluiSubShow || timeOutGame) return;
	
	if (state == GLUT_DOWN){ // mouse click
		findNearAndFarPoint(x, y);
		Point3D p;
		int sName, cx, cy, cz;
		double pValue;
		if (findIntersectionWithRubikFace(p, sName, cx, cy, cz, pValue)){ // click in rubik
			if (!isRotating && ! isShuffling){
				isCheckingRubik = true;
				clickPoint.set(p);
				sliceName = sName;
				cubeX = cx; cubeY = cy; cubeZ = cz;
				planeValue = pValue;
			}
		}else { // click in backgroud
			isCheckingCamera = true;
			camX = x; camY = y;
		}
	}else{ // mouse release
		isCheckingRubik = false;
		isCheckingCamera = false;
	}
}

Point3D findIntersectionWithPlane(int name, double value){
	double dx, dy, dz, t, px, py, pz;

	// init the line cross near point and far point
	dx = farPoint.getX() - nearPoint.getX();
	dy = farPoint.getY() - nearPoint.getY();
	dz = farPoint.getZ() - nearPoint.getZ();

	if (name == SLICE_X){
		t = (value - nearPoint.getX()) / dx;
		px = value;
		py = nearPoint.getY() + t * dy;
		pz = nearPoint.getZ() + t * dz;
	}else if (name == SLICE_Y){
		t = (value - nearPoint.getY()) / dy;
		px = nearPoint.getX() + t * dx;
		py = value;
		pz = nearPoint.getZ() + t * dz;
	}else{
		t = (value - nearPoint.getZ()) / dz;
		px = nearPoint.getX() + t * dx;
		py = nearPoint.getY() + t * dy;
		pz = value;
	}
	
	return Point3D(px, py, pz);
}

void rotateRubik(int ignored){
	if (rotatedAngle + abs(rotateAngle) < 90.0){
		myRubik.rotate(rotateSliceName, rotateSliceValue, rotateAngle);
		rotatedAngle += abs(rotateAngle);
		glutPostRedisplay();
		glutTimerFunc(1, rotateRubik, 0);
	}else{
		double rest = 90.0 - rotatedAngle;
		if (rotateAngle < 0) rest = -rest;
		myRubik.rotate(rotateSliceName, rotateSliceValue, rest);
		glutPostRedisplay();
		rotatedAngle = 0.0;
		isRotating = false;
		int dir = (rotateAngle < 0) ? CLOCKWISE : COUNTER_CLOCKWISE;
		myRubik.invertSlice(rotateSliceName, rotateSliceValue, dir);
		cout << myRubik.isCorrect() << endl;
	}
}

void rotateRubik(Point3D a, Point3D b){
	double absX = abs(b.getX() - a.getX());
	double absY = abs(b.getY() - a.getY());
	double absZ = abs(b.getZ() - a.getZ());
	
	// -speed ~ CLOCKWISE ; speed ~ COUNTER_CLOCKWISE
	if (sliceName == SLICE_X){
		if (absZ > absY){
			rotateSliceName = SLICE_Y;
			rotateSliceValue = cubeY;
			if (cubeX == 0) rotateAngle = (b.getZ() > a.getZ()) ? speed : -speed;
			else rotateAngle = (b.getZ() > a.getZ()) ? -speed : speed;
		}else{
			rotateSliceName = SLICE_Z;
			rotateSliceValue = cubeZ;
			if (cubeX == 0) rotateAngle = (b.getY() > a.getY()) ? -speed : speed;
			else rotateAngle = (b.getY() > a.getY()) ? speed : -speed;
		}
	}else if (sliceName == SLICE_Y){
		if (absZ > absX){
			rotateSliceName = SLICE_X;
			rotateSliceValue = cubeX;
			if (cubeY == 0) rotateAngle = (b.getZ() > a.getZ()) ? -speed : speed;
			else rotateAngle = (b.getZ() > a.getZ()) ? speed : -speed;
		}else{
			rotateSliceName = SLICE_Z;
			rotateSliceValue = cubeZ;
			if (cubeY == 0) rotateAngle = (b.getX() > a.getX()) ? speed : -speed;
			else rotateAngle = (b.getX() > a.getX()) ? -speed : speed;
		}
	}else{
		if (absX > absY){
			rotateSliceName = SLICE_Y;
			rotateSliceValue = cubeY;
			if (cubeZ == 0) rotateAngle = (b.getX() > a.getX()) ? -speed : speed;
			else rotateAngle = (b.getX() > a.getX()) ? speed : -speed;
		}else{
			rotateSliceName = SLICE_X;
			rotateSliceValue = cubeX;
			if (cubeZ == 0) rotateAngle = (b.getY() > a.getY()) ? speed : -speed;
			else rotateAngle = (b.getY() > a.getY()) ? -speed : speed;
		}
	}

	glutTimerFunc(1, rotateRubik, 0);
}

void myMotion(int x, int y){
	findNearAndFarPoint(x, y);
	
	if (isCheckingRubik){
		Point3D p = findIntersectionWithPlane(sliceName, planeValue);
		if (manhattanDistance(p, clickPoint) > 0.7){ // start rotate
			isCheckingRubik = false;
			isRotating = true;
			rotateRubik(clickPoint, p);
		}
	}else if (isCheckingCamera){
		int dx = x - camX, dy = y - camY;
		myCam.up(dy * 0.5);
		myCam.right(dx * 0.5);
		camX = x; camY = y;
		glutPostRedisplay();
	}
}

void shuffleRubik(int ignored){
	if (rotatedAngle + abs(rotateAngle) < 90){
		myRubik.rotate(rotateSliceName, rotateSliceValue, rotateAngle);
		rotatedAngle += abs(rotateAngle);
		glutPostRedisplay();
		glutTimerFunc(1, shuffleRubik, 0);
	}else{
		double rest = 90.0 - rotatedAngle;
		if (rotateAngle < 0) rest = -rest;
		myRubik.rotate(rotateSliceName, rotateSliceValue, rest);
		glutPostRedisplay();
		int dir = (rotateAngle < 0) ? CLOCKWISE : COUNTER_CLOCKWISE;
		myRubik.invertSlice(rotateSliceName, rotateSliceValue, dir);
		
		rotatedAngle = 0;
		shuffleCounter++;
		if (shuffleCounter == shuffleNum){
			isShuffling = isRotating = false;
			speed /= 2;
		}else{	
			rotateSliceName = rand() % 3;
			rotateSliceValue = rand() % n;
			rotateAngle = (rand() % 2 == 0) ? speed : -speed;
			glutTimerFunc(0, shuffleRubik, 0);
		}
	}
}

void shuffleRubik() {
	isShuffling = isRotating = true;
	shuffleNum = 12 * n; shuffleCounter = 0;
	speed *= 2; // double speed when shuffling
	
	srand(time(NULL));
	rotateSliceName = rand() % 3;
	rotateSliceValue = rand() % n;
	rotateAngle = (rand() % 2 == 0) ? speed : -speed;
	
	glutTimerFunc(0, shuffleRubik, 0);
}

void mySpecial(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP: myCam.slide(0, 0, -0.2); break;
		case GLUT_KEY_DOWN: myCam.slide(0, 0, 0.2); break;
		case GLUT_KEY_LEFT: myCam.rotate(5.0); break;
		case GLUT_KEY_RIGHT: myCam.rotate(-5.0); break;
	}
	
	glutPostRedisplay();
}

void gameTimer(int v) {
	string base = "Time remaining (in seconds): ";
    char* buf = new char;
    string s = base + itoa(timer * 10- currentTime, buf, 10);
    timeRemain->set_text(s.c_str());
    if (!isShuffling) currentTime += 1;
    if (currentTime > timer * 10) {
    	timeOutGame = true;
       	timeRemain->set_text("Time out! Press restart to start again!");
       	delete buf;
      	return;
    }
    glutPostRedisplay();
  	glutTimerFunc(v, gameTimer, v);
}

void myReshape(int x, int y) {
	int tx, ty, tw, th;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
    if (th == 0) th = 1;
    float ratio = 1.0 * tw / th;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
	glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, tw, th);

    // Set the correct perspective.
    gluPerspective(45, ratio, 1, 1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void controlCallback(int control) {
	int tx, ty, tw, th;
	switch (control) {
		case RESET_GAME_ID:
			myInit();
			GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
			myReshape(tx, ty);
			shuffleRubik();
			if (timeModeGame) {
				currentTime = 0;
				timeRemain->enable();
				gameTimer(1000);
			}
			break;
		case START_NEW_GAME_ID:		
			gluiMain->show();
			gluiMainShow = true;
			
			gluiSub->hide();
			gluiSubShow = false;
			
			n = cubeSize;
			myInit();
			GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
			myReshape(tx, ty);
			shuffleRubik();
			if (timeModeGame) {
				currentTime = 0;
				timeRemain->enable();
				gameTimer(1000);
			} 
	
			break;
		case RESTART_GAME_ID:
			gluiMain->hide();
			gluiMainShow = false;
			
			gluiSub->show();
			gluiSubShow = true;
			
			myInit();
			
			currentTime = 0;
			break;
		case GAME_MODE_ID:
			modeGroup->get_int_val() == 0 ? timeModeGame = true : timeModeGame = false;
			if (timeModeGame) spinnerTimer->enable();
			else spinnerTimer->disable();
			break;
		case SPEED_ID:
			rotateAngle = (rotateAngle > 0) ? speed : -speed;
			break;
		case HELP_ID:
			helper.set(screenWidth, screenHeight);
			helper.draw();
			
	}
	glutPostRedisplay();
}

void myGlutIdle( void ){
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if(glutGetWindow() != mainWindow) {
  	glutSetWindow(mainWindow); 	
  }

//  GLUI_Master.sync_live_all();
  glutPostRedisplay();
}

void startScene() {
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int sW = 200;
	int sH = 300;	
	/** Create the side subwindow **/
   	GLUI *glui = GLUI_Master.create_glui_subwindow( mainWindow );
   	
   	glui->add_statictext("Setup your RubiKA Game");
   	glui->add_separator();
   	
   	glui->add_statictext("");
   	
   	GLUI_Panel *mode = new GLUI_Panel(glui, "Game mode" );
   	modeGroup = new GLUI_RadioGroup(mode, &showRadio, GAME_MODE_ID, controlCallback);
   	new GLUI_RadioButton(modeGroup, "Time mode");
   	new GLUI_RadioButton(modeGroup, "Classic mode");
   	modeGroup->set_alignment(GLUI_ALIGN_CENTER);
   	
   	spinnerTimer = new GLUI_Spinner(glui, "Set your timer (in minutes):", &timer);
   	spinnerTimer->set_int_limits(1, 30);
   	spinnerTimer->set_alignment(GLUI_ALIGN_CENTER);
   	if (timeModeGame) spinnerTimer->enable();
	else spinnerTimer->disable();
   	
   	glui->add_statictext("");
   	
   	GLUI_Spinner *cubeSizeSpinner = new GLUI_Spinner(glui, "Cube's size:", &cubeSize);
   	cubeSizeSpinner->set_int_limits(2, 10);
   	cubeSizeSpinner->set_alignment(GLUI_ALIGN_CENTER);
   	
   	glui->add_statictext("");

	new GLUI_Button(glui, "START GAME", START_NEW_GAME_ID, controlCallback);
	
	glui->add_statictext("");
	glui->add_separator();
	glui->add_statictext("Note: Press f for toggle screen mode");
	
	gluiSub = glui;
}

void mainScene() {
	GLUI_Spinner *speedSpinner;
	GLUI_Panel *newGamePanel, *setPanel;
	/** Create the side subwindow **/
   	GLUI *glui = GLUI_Master.create_glui_subwindow( mainWindow );
   	
   	glui->add_statictext("RubiKA Game");
   	glui->add_separator();
   	
   	glui->add_statictext("");

	/** Reset game button **/
	new GLUI_Button(glui, "Shuffle Rubik", RESET_GAME_ID, controlCallback);
	glui->add_statictext("");
	new GLUI_Button(glui, "RESTART GAME", RESTART_GAME_ID, controlCallback);
	
	/** Setting section **/
	setPanel = new GLUI_Panel(glui, "Setting", false);
	new GLUI_StaticText(setPanel, "");
	GLUI_StaticText *spinText = new GLUI_StaticText(setPanel, "Spin Speed");
	spinText->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_Scrollbar *speedBar = new GLUI_Scrollbar(setPanel, "Spin speed:", GLUI_SCROLL_HORIZONTAL, &speed, SPEED_ID, controlCallback);
	speedBar->set_float_limits(0.1, 6);
	new GLUI_StaticText(setPanel, "");
	
	new GLUI_Checkbox(setPanel, "Enable Sound", &enableSound);
	
	/** A 'help' button **/
	new GLUI_Button(glui, "Help", HELP_ID, controlCallback);
	new GLUI_StaticText( glui, "" );

	/** A 'quit' button **/
	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);
	
	/** Link windows to GLUI, and register idle callback **/
	glui->set_main_gfx_window( mainWindow );
	
	glui->add_statictext("");
	glui->add_separator();
	glui->add_statictext("Note: Press f for toggle screen mode");
	
	glui->add_statictext("");
	timeRemain = new GLUI_StaticText(glui, "Time remaining (in seconds): ");
	
	gluiMain = glui;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(screenPosX, screenPosY);
	glutInitWindowSize(screenWidth, screenHeight);
	mainWindow = glutCreateWindow(screenTitle);
	
	GLUI_Master.set_glutReshapeFunc(myReshape);
	GLUI_Master.set_glutDisplayFunc(myDisplay);
	GLUI_Master.set_glutKeyboardFunc(myKeyboard);
	GLUI_Master.set_glutSpecialFunc(mySpecial);
	GLUI_Master.set_glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	
	myInit(); 
	
	mainScene();
	gluiMain->hide();
	gluiMainShow = false;
	
	startScene();
	gluiSub->show();
	gluiSubShow = true;

	/**** We register the idle callback with GLUI, *not* with GLUT ****/
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
	
	glutMainLoop();
	return 0;
}
