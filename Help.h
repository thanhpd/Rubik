#ifndef RUBIK_HELP_H
#define RUBIK_HELP_H

#ifdef __APPLE__

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include "Image.h"
#include <string>

using std::string;

class Help {
	private:
		GLuint textureId;
		int width;
		int height;
		GLuint loadTexture(string imageName);
		GLuint loadTexture(Image*);
	public:
		Help();
		Help(int width, int height);
		void set(int width, int height);
		void draw();
		GLuint getTextureId();
};

#endif //RUBIK_FACE_H

